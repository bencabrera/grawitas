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
    
    while(std::getline(input,line))
    {
        // remove leading whitespaces and split string by comma
        boost::trim(line);
        
        std::vector<std::string> titleNDate;
        std::string segment;
        
        // convert line back to stringstream for getline
        std::istringstream lineS(line);
        while(std::getline(lineS, segment, ','))
        {
           // not sure if this will store Date in titleNDate
           titleNDate.push_back(segment); // splits string at ',' character
        }
        
        if (titleNDate.empty() || titleNDate.size() == 1){
            if (previousDate == "NULL"){
                throw std::invalid_argument("Input talk page file does not contain any dates seperated for their titles seperated by a comma. Aborting.");
            }
            else{
                titleNDate[1] = previousDate;
                lines.insert(std::pair<std::string, std::string>(titleNDate[0], titleNDate[1]));
                continue;
            }
        }
        
        if ((titleNDate[1].find('/') == std::string::npos) || (titleNDate[0].find('/') != std::string::npos)) {
            // checks correct format
            throw std::invalid_argument("Input talk page format should be 'title,dd/mm/yyyy'. Aborting.");
        } else {
            lines.insert(std::pair<std::string, std::string>(titleNDate[0], titleNDate[1]));
        }
        
        return lines;
    }
}

