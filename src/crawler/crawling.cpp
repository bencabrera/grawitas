#include "crawling.h"

#include <vector>
#include <map>
#include <iostream>
#include <set>
#include <stack>
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>

#include "requestRawPagesFromWikipedia.h"
#include "../core/parsing/coreTalkPageParsing.h"

TalkPageTitle parse_page_title(std::string title);

std::vector<std::string> generate_next_titles_to_get(const std::vector<TalkPageResult>& results)
{
	std::map<std::string,int> article_progress_map;
	for(const auto& result : results)
	{
		auto split = parse_page_title(result.title);
		const auto& title = split.title;
		const auto& i_archive = (split.is_archive) ? split.i_archive : 0;

		if(result.missing) {
			article_progress_map[title] = -1;
			continue;
		}

	   	if(article_progress_map.count(title)) {
			if(article_progress_map[title] == -1)
				continue;

			article_progress_map[title] = std::max(article_progress_map[title], i_archive);
		}
		else 
			article_progress_map[title] = i_archive;
	}

	std::vector<std::string> next_titles;
	for(const auto& p : article_progress_map) 
	{
		if(p.second == -1)
			continue;	

		for(int i = 0; i < N_NEXT_ARCHIVES; i++)
			next_titles.push_back(p.first + "/Archive " + std::to_string(p.second+i+1));
	}

	return next_titles;
}


TalkPageTitle parse_page_title(std::string title)
{
	TalkPageTitle tmp;

	if(title.substr(0,5) == "Talk:")
		title = title.substr(5);

	auto pos = title.find("/Archive");
	tmp.is_archive = (pos != std::string::npos);

	if(tmp.is_archive)
	{
		tmp.title = title.substr(0,pos);
		boost::trim(tmp.title);
		std::string archive_str = title.substr(pos+9);
		tmp.i_archive = std::stol(archive_str);
	}
	else
	{
		tmp.title = title;
	}

	return tmp;
}


void export_finished_talk_page(const std::string& output_folder, const std::set<Grawitas::Format> formats, const std::string& title, const Grawitas::ParsedTalkPage& parsed_talk_page)
{
	std::string title_filename = Grawitas::safeEncodeTitleToFilename(title);
	std::map<Grawitas::Format, std::string> formats_with_paths;
	for (const auto& format : formats)
		formats_with_paths.insert({ format, output_folder + "/" + title_filename + Grawitas::FormatFileExtensions.at(format) });

	Grawitas::output_in_formats_to_files(formats_with_paths, parsed_talk_page);
}

void crawling(const std::vector<std::string>& article_titles, const std::string& output_folder, const std::set<Grawitas::Format> formats)
{
	// initialization of data structures
	std::map<std::string, Grawitas::ParsedTalkPage> partially_parsed_articles_map;

	std::vector<std::string> next_pages_to_crawl;
	for(auto title : article_titles)
		next_pages_to_crawl.push_back(title);

	while(!next_pages_to_crawl.empty())
	{
		// get the next n titles
		std::vector<std::string> current_titles;
		std::size_t n_next_pages = std::min(next_pages_to_crawl.size(),static_cast<std::size_t>(N_PAGES_PER_REQUEST)+1);
		for(std::size_t i = 0; i < n_next_pages; i++)
		{
			current_titles.push_back(next_pages_to_crawl.back());
			next_pages_to_crawl.pop_back();
		}
		for (const auto& title : current_titles)
			std::cout << "Downloading '" << title << "'." << std::endl;
		

		// request the talk pages from wikipedia
		const auto results = request_raw_pages_from_wikipedia(current_titles);

		// add page titles to crawl next
		const auto to_add = generate_next_titles_to_get(results);
		for(auto it = to_add.rbegin(); it != to_add.rend(); it++)
			next_pages_to_crawl.push_back(*it);

		// parse every talk page and add it to partially_parsed_articles
		for(const auto& result : results) 
		{
			std::cout << "Parsing '" << result.title << "'." << std::endl;
			const auto split = parse_page_title(result.title);
			auto parsed = Grawitas::parse_talk_page(result.content);
			auto& list = partially_parsed_articles_map[split.title];
        	list.splice(list.end(), parsed);
		}

		// export those articles that are finished and remove them
		for(const auto& result : results) 
		{
			const auto split = parse_page_title(result.title);
			if(result.missing && partially_parsed_articles_map.count(split.title)) {
				std::cout << "Finished all archives of '" << split.title << "'. Exporting results." << std::endl;

				// remove all remaining once from next_pages_to_crawl
				next_pages_to_crawl.erase(std::remove_if(next_pages_to_crawl.begin(), next_pages_to_crawl.end(), [&split](const std::string& str) { 
					auto tmp = parse_page_title(str);
					return tmp.title == split.title;
				}), next_pages_to_crawl.end());

				auto& parsed_talk_page = partially_parsed_articles_map[split.title];

				std::size_t cur_id = 1;
				for (auto& sec : parsed_talk_page) 
					calculate_ids(sec.second, cur_id);

				export_finished_talk_page(output_folder, formats, split.title, parsed_talk_page);
				partially_parsed_articles_map.erase(split.title);
			}
		}
	}
}
