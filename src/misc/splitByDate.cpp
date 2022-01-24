#include "splitByDate.h"

#include "../talkPageParser/date.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/erase.hpp>






void export_finished_pages(const std::string& output_folder, const std::set<Grawitas::Format> formats, const std::string& title, const std::vector<Grawitas::Comment>& parsed_talk_page, const std::string& dateToSplit)
{
        
        std::string title_filename = Grawitas::safeEncodeTitleToFilename(title);
        //change dateToSplit so date appears correctly in output file title
        std::string fileNameDate = dateToSplit;
        boost::erase_all(fileNameDate, "/");
        std::string title_filename_after_date = Grawitas::safeEncodeTitleToFilename(title + "_after_" + fileNameDate);
            
        std::map<Grawitas::Format, std::string> formats_with_paths;
        
        std::vector<Grawitas::Comment> before_date_comments;
        std::vector<Grawitas::Comment> after_date_comments;
        
        for(auto& comment : parsed_talk_page) {
            Grawitas::Date dateToCompare = comment.Date;
            Grawitas::Date dateToSplitD = Grawitas::make_date(0,0,0, std::stoi(dateToSplit.substr(1,2)), std::stoi(dateToSplit.substr(4,2)), std::stoi(dateToSplit.substr(7,4)));
            
            if(!(dateToSplitD >= dateToCompare)){
                after_date_comments.push_back(comment);
            }
            else {
                // add to data structure of talk page before the date
                before_date_comments.push_back(comment);
                
            }
        }
            
        if(after_date_comments.size() == 0 || after_date_comments.empty()){
            std::cout << "There are no comments made for " << title << " after this date: " << dateToSplit << ".\n";
        }
        else {
            // export comments after date to file
            for (const auto& format : formats){
                formats_with_paths.insert({ format, output_folder + "/" + title_filename_after_date + Grawitas::FormatFileExtensions.at(format) });
                Grawitas::output_in_formats_to_files(formats_with_paths, after_date_comments, {"id", "parent_id", "user", "date", "section", "text"});
            }
            formats_with_paths.clear();
        }
        for (const auto& format : formats){
            formats_with_paths.insert({ format, output_folder + "/" + title_filename + Grawitas::FormatFileExtensions.at(format) });
            Grawitas::output_in_formats_to_files(formats_with_paths, before_date_comments, {"id", "parent_id", "user", "date", "section", "text"});
        }
        
}
