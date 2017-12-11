#include "readLinesFromFile.h"
#include <boost/algorithm/string/trim.hpp>

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

