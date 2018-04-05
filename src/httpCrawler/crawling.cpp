#include "crawling.h"

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <set>
#include <stack>
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include "getPagesFromWikipedia.h"
#include "../talkPageParser/parsing.h"

namespace {

	std::tuple<std::string,bool, int> parse_page_title(std::string title)
	{
		std::tuple<std::string,bool, int> t{"", false, 0};

		if(boost::to_lower_copy(title.substr(0,5)) == "talk:")
			title = title.substr(5);

		if(boost::to_lower_copy(title.substr(0,10)) == "user talk:")
			title = title.substr(10);

		if(boost::to_lower_copy(title.substr(0,10)) == "user_talk:")
			title = title.substr(10);

		auto pos = title.find("/Archive");
		std::get<1>(t) = (pos != std::string::npos);

		if(std::get<1>(t))
		{
			std::get<0>(t) = title.substr(0,pos);
			boost::trim(std::get<0>(t));
			std::string archive_str = title.substr(pos+9);
			std::get<2>(t) = std::stol(archive_str);
		}
		else
			std::get<0>(t) = title;

		return t;
	}

	// std::vector<std::string> generate_next_titles_to_get(const std::vector<TalkPageResult>& results)
	// {
		// std::map<std::string,int> article_progress_map;
		// for(const auto& result : results)
		// {
			// const auto& title = result.title;
			// const auto& i_archive = (result.is_archive) ? result.i_archive : 0;

			// if(result.missing) {
				// article_progress_map[title] = -1;
				// continue;
			// }

			// if(article_progress_map.count(title)) {
				// if(article_progress_map[title] == -1)
					// continue;

				// article_progress_map[title] = std::max(article_progress_map[title], i_archive);
			// }
			// else 
				// article_progress_map[title] = i_archive;
		// }

		// for(auto m: article_progress_map)
			// std::cout << m.first << " ---> " << m.second << std::endl;

		// std::vector<std::string> next_titles;
		// for(const auto& p : article_progress_map) 
		// {
			// if(p.second == -1)
				// continue;	

			// for(int i = 0; i < Grawitas::N_NEXT_ARCHIVES; i++)
				// next_titles.push_back(p.first + "/Archive " + std::to_string(p.second+i+1));
		// }

		// return next_titles;
	// }



	void export_finished_talk_page(const std::string& output_folder, const std::set<Grawitas::Format> formats, const std::string& title, const std::vector<Grawitas::Comment>& parsed_talk_page)
	{
		std::string title_filename = Grawitas::safeEncodeTitleToFilename(title);
		std::map<Grawitas::Format, std::string> formats_with_paths;
		for (const auto& format : formats)
			formats_with_paths.insert({ format, output_folder + "/" + title_filename + Grawitas::FormatFileExtensions.at(format) });

		Grawitas::output_in_formats_to_files(formats_with_paths, parsed_talk_page, {"id", "parent_id", "user", "date", "section", "text"});
	}

	// void sanitize_titles(std::vector<std::string>& titles) 
	// {
		// if(titles.empty())
			// throw std::invalid_argument("Input talk page file does not contain any uncommented, non-empty lines. Aborting.");

		// for(auto& title : titles)
		// {
			// if(title.substr(0,7) == "http://")
				// throw std::invalid_argument("It seems like the provided articles are URLs. In newer versions of Grawitas you should simply provide the titles of the articles in the input file.");

			// if(boost::to_lower_copy(title.substr(0,5)) == "talk:")
				// title = title.substr(5);
		// }
	// }	
}

namespace Grawitas {
	void crawling(std::vector<std::string> article_titles, const std::string& output_folder, const std::set<Grawitas::Format> formats, AdditionalCrawlerOptions options) 
	{
		// ::sanitize_titles(article_titles);

		// initialization of data structures
		std::map<std::string, std::vector<Grawitas::Comment>> partially_parsed_articles_map;
		std::deque<std::pair<std::string, int>> page_progress;

		for(auto title : article_titles)
		{
			// auto tmp = parse_page_title(title);
			// page_progress.emplace_back(std::get<0>(tmp), 0);
			page_progress.emplace_back(title, 0);
		}

		while(!page_progress.empty() && (options.abort == nullptr || !(*options.abort)))
		{
			// get the next n titles
			std::vector<std::string> current_titles;
			for(std::size_t i = 0; i < N_PAGES_PER_REQUEST; i++)
			{
				auto& page = page_progress[i % page_progress.size()];
				if(page.second == 0)
					current_titles.push_back(page.first);
				else
					current_titles.push_back(page.first + "/Archive " + std::to_string(page.second));
				page.second++;
			}
			for (const auto& title : current_titles)
				if(options.status_callback)
					options.status_callback("Downloading '" + title + "'.");


			// request the talk pages from wikipedia
			const auto results = get_pages_from_wikipedia(current_titles);

			// if options.keep_raw_talk_pages is set export the page to a file
			if(options.keep_raw_talk_pages) {
				for(const auto& result : results) 
				{
					if(result.missing)
						continue; 

					std::ofstream raw_file;
					if(!result.is_archive)
						raw_file.open(output_folder + "/" + Grawitas::safeEncodeTitleToFilename(result.title) + ".wikitext");
					else
						raw_file.open(output_folder + "/" + Grawitas::safeEncodeTitleToFilename(result.title) + ".wikitext",std::ios::app);
					raw_file << result.content << std::endl;
				}
			}

			// parse every talk page and add it to partially_parsed_articles
			for(const auto& result : results) 
			{
				if(result.missing)
					continue; 

				if(options.status_callback)
					options.status_callback("Parsing '" + result.full_title + "'.");

				auto parsed = Grawitas::parse_talk_page(result.content);
				auto& vec = partially_parsed_articles_map[result.title];
				vec.insert(vec.end(), parsed.begin(), parsed.end());
			}

			// remove pages that returned missing from page_progress
			for(const auto& result : results) 
			{
				if(!result.missing)
					continue;

				// remove all remaining once from next_pages_to_crawl
				page_progress.erase(std::remove_if(page_progress.begin(), page_progress.end(), [&result](const std::pair<std::string,int>& page) { 
					auto tmp = parse_page_title(page.first);
					return std::get<0>(tmp) == result.title;
				}), page_progress.end());
			}

			// export those articles that are finished and remove them
			for(const auto& result : results) 
			{
				if(result.missing && partially_parsed_articles_map.count(result.title)) {
					if(options.status_callback)
						options.status_callback("Finished all archives of '" + result.title + "'. Exporting results.");

					auto& parsed_talk_page = partially_parsed_articles_map[result.title];

					std::size_t cur_id = 1;
					calculate_ids(parsed_talk_page, cur_id);

					export_finished_talk_page(output_folder, formats, result.title, parsed_talk_page);
					partially_parsed_articles_map.erase(result.title);
				}
			}
		}
	}
}
