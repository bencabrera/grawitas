#include "splitByDate.h"

#include <string>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>





void export_finished_pages(const std::string& output_folder, const std::set<Grawitas::Format> formats, const std::string& title, const std::vector<Grawitas::Comment>& parsed_talk_page, const std::string& dateToSplit)
{
        std::string title_filename = Grawitas::safeEncodeTitleToFilename(title);
        std::string title_filename_after_date = Grawitas::safeEncodeTitleToFilename(title + "_after_" + dateToSplit);
        
        std::map<Grawitas::Format, std::string> formats_with_paths;
        
        std::vector<Grawitas::Comment> before_date_comments = parsed_talk_page;
        std::vector<Grawitas::Comment> after_date_comments;
        
        for(auto& comment : before_date_comments) {
            std::tm dateToCompare = boost::trim(comment.Date);
            std::tm splitDate = 
            
            if((dateToCompare.substr(0,2) <= dateToSplit.substr(0,2)) && (dateToCompare.substr(3,5) <= dateToSplit.substr(3,5)) && (dateToCompare.substr(6,10) <= dateToCompare.substr(6,10))){
                
                continue;
            }
            else {
                // add to data structure of talk page after the date
                after_date_comments.push_back(comment);
                
                // delete this comment from 'before date' file
                before_date_comments.erase(std::remove_if(before_date_comments.begin(), before_date_comments.end(), [&comment](const Grawitas::Comment& pageComment) {
                    return comment.Id == pageComment.Id;
                }), before_date_comments.end());
            }
            
            }
            
        if(after_date_comments.size() == 0){
            std::cout << "There are no comments made for " << title << " after this date: " << dateToSplit << ".\n";
        }
        else {
            // export comments after date to file
            for (const auto& format : formats){
                formats_with_paths.insert({ format, output_folder + "/" + title_filename_after_date + Grawitas::FormatFileExtensions.at(format) });
                Grawitas::output_in_formats_to_files(formats_with_paths, after_date_comments, {"id", "parent_id", "user", "date", "section", "text"});
            }
        }
        for (const auto& format : formats){
            formats_with_paths.insert({ format, output_folder + "/" + title_filename + Grawitas::FormatFileExtensions.at(format) });
            Grawitas::output_in_formats_to_files(formats_with_paths, before_date_comments, {"id", "parent_id", "user", "date", "section", "text"});
        }
        
}
