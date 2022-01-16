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
#include "../misc/splitByDate.h"

namespace {

	std::vector<std::string> generate_next_titles_to_get(const std::vector<TalkPageResult>& results)
	{
		std::map<std::string,int> article_progress_map;
		for(const auto& result : results)
		{
			const auto& title = result.title;
			const auto& i_archive = (result.is_archive) ? result.i_archive : 0;

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

			for(int i = 0; i < Grawitas::N_NEXT_ARCHIVES; i++)
				next_titles.push_back(p.first + "/Archive " + std::to_string(p.second+i+1));
		}

		return next_titles;
	}



	void export_finished_talk_page(const std::string& output_folder, const std::set<Grawitas::Format> formats, const std::string& title, const std::vector<Grawitas::Comment>& parsed_talk_page)
	{
		std::string title_filename = Grawitas::safeEncodeTitleToFilename(title);
		std::map<Grawitas::Format, std::string> formats_with_paths;
		for (const auto& format : formats)
			formats_with_paths.insert({ format, output_folder + "/" + title_filename + Grawitas::FormatFileExtensions.at(format) });
        

		Grawitas::output_in_formats_to_files(formats_with_paths, parsed_talk_page, {"id", "parent_id", "user", "date", "section", "text"});
	}

	void sanitize_titles(std::vector<std::string>& titles) 
	{
		if(titles.empty())
			throw std::invalid_argument("Input talk page file does not contain any uncommented, non-empty lines. Aborting.");

		for(auto& title : titles)
		{
			if(title.substr(0,7) == "http://")
				throw std::invalid_argument("It seems like the provided articles are URLs. In newer versions of Grawitas you should simply provide the titles of the articles in the input file.");

			if(boost::to_lower_copy(title.substr(0,5)) == "talk:")
				title = title.substr(5);
            
            // removes any underscores used as spaces in page titles
            std::string delim = "_";
            auto end = title.find(delim);
            if (end != std::string::npos)
                std::replace(title.begin(), title.end(), '_', ' ');
		}
	}
    void sanitize_titles(std::map<std::string, std::string>& titlesNDates)
    {
        std::map<std::string, std::string> newTitlesNDates;
        for (auto& titleNDate : titlesNDates ) {
            auto title = titleNDate.first;
            auto date = titleNDate.second;
            if(title.substr(0,7) == "http://")
                throw std::invalid_argument("It seems like the provided articles are URLs. In newer versions of Grawitas you should simply provide the titles of the articles in the input file.");

            if(boost::to_lower_copy(title) == "talk:")
                title = title.substr(5);
            
            
            // removes any underscores used as spaces in page titles
            std::string delim = "_";
            auto end = title.find(delim);
            if (end != std::string::npos)
                std::replace(title.begin(), title.end(), '_', ' ');
            
            std::pair<std::string, std::string> newTitleNDate = std::make_pair(title, date);
            newTitlesNDates.insert(newTitleNDate);
            
        }
        // replaces titlesNDates with sanitised map of newTitlesNDates
        newTitlesNDates.swap(titlesNDates);
    }




}

namespace Grawitas {
	void crawling(std::vector<std::string> article_titles, std::map<std::string, std::string> titleNDates, const std::string& output_folder, const std::set<Grawitas::Format> formats, AdditionalCrawlerOptions options)
	{
                
		::sanitize_titles(article_titles);
        ::sanitize_titles(titleNDates);
		// initialization of data structures
		std::map<std::string, std::vector<Grawitas::Comment>> partially_parsed_articles_map;
		std::deque<std::pair<std::string, int>> page_progress;

		for(auto title : article_titles)
			page_progress.emplace_back(title, 0);

		while(!page_progress.empty() && (options.abort == nullptr || !(*options.abort)))
		{
			// get the next n titles
			std::vector<std::string> current_titles;
			for(std::size_t i = 0; i < N_PAGES_PER_REQUEST; i++)
			{
				auto& page = page_progress[i % page_progress.size()];
				if(page.second == 0)
					current_titles.push_back("Talk:" + page.first);
				else
					current_titles.push_back("Talk:" + page.first + "/Archive " + std::to_string(page.second));
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
					return page.first == result.title;
				}), page_progress.end());
                
                // additional condition for when there is only one page remaining and it is a missing page 
                if((page_progress.size() == 1))
                    page_progress.clear();
                
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
                    
                    // split by date if requested
                    if(options.split_by_date){
                        // get date for title
                        std::string dateToSplit;
                        try {
                            dateToSplit = titleNDates[result.title];
                        }
                        catch (const std::out_of_range&) {
                            dateToSplit = "01/01/2000";
                        }
                        options.status_callback("Finished all archives of '" + result.title + " after" + dateToSplit + "'. Exporting results.");
                        export_finished_pages(output_folder, formats, result.title, parsed_talk_page, dateToSplit);
                    }
                    else {
                        export_finished_talk_page(output_folder, formats, result.title, parsed_talk_page);
                    }
                    
                    partially_parsed_articles_map.erase(result.title);
				}
			}
		}
	}
}
