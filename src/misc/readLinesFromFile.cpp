#include "readLinesFromFile.h"
#include <boost/algorithm/string/trim.hpp>
#include <sstream>
#include <iostream>

std::vector<std::string> read_lines_from_file(std::istream& input)
{
    std::vector<std::string> lines;
    std::string line;
    while(std::getline(input,line))
    {
        boost::trim(line);
		if(!line.empty() && line.front() != '#')
        	lines.push_back(line);
    }

	return lines;
}

std::map<std::string, std::string> read_lines_dates_from_file(std:: istream& input)
{
    std::map<std::string, std::string> lines;
    std::string line;
    std::string previousDate = "NULL";
    
    while(std::getline(input,line, '\n'))
    {
        // remove leading whitespaces and split string by comma
        boost::trim(line);
        
        std::pair<std::string, std::string> titleNDate;
        
        // convert line back to stringstream for getline
        int i = 0;
        size_t pos = 0;
        std::string delimiter = ",";
        std::string title;
        while((pos = line.find(delimiter)) != std::string::npos) {
            title = line.substr(0, pos);
            line.erase(0, pos + delimiter.length());
        }
        titleNDate = std::make_pair(title, line);
        if (titleNDate.first.empty()){
            if (previousDate == "NULL"){
                throw std::invalid_argument("Input talk page file does not contain any dates seperated for their titles seperated by a comma. Aborting.");
            }
            else {
                // due to way string splitting works the title is stored in the date position
                // if there is no date
                titleNDate.first = titleNDate.second;
                titleNDate.second = previousDate;
                lines.insert(titleNDate);
                continue;
            }
        }
        
        if (titleNDate.second.find('/') == std::string::npos || titleNDate.first.find('/') != std::string::npos) {
             throw std::invalid_argument("Input talk page format should be 'title,dd/mm/yyyy'. Aborting.");
        }
        lines.insert(titleNDate);
        previousDate = titleNDate.second;
    }
    return lines;
}

