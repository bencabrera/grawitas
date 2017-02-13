#include "listHumanReadableOutput.h"

#include <algorithm>
#include <iomanip>
#include "dateOutput.h"

namespace Grawitas {

	const std::string HEADER_DEVIDER = "================================================================================================================================";
	const std::string COMMENT_DEVIDER = "..............................................................................................................................................................................................................................................................................";

	void listToHumanReadable(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage)
	{
		for (auto paragraph : parsedTalkPage) {
			
			// write header of the current paragraph
			ostr << HEADER_DEVIDER << std::endl;
			for(auto it = paragraph.first.begin(); it != paragraph.first.end(); ++it) 
				ostr << *it << (it+1 != paragraph.first.end() ? " >> " : "");
			ostr << std::endl << HEADER_DEVIDER << std::endl;

			for(auto it = paragraph.second.begin(); it != paragraph.second.end(); ++it)
			{
				auto comment = *it;

				ostr
					<< std::setw(8) << comment.Id << " | "
					<< std::setw(8) << comment.ParentId << " | "
					<< std::setw(32) << comment.User << " | "
					<< std::setw(40) << to_human_readable(comment.Date) << " | "
					<< comment.Text;
			}
			ostr << std::endl << COMMENT_DEVIDER << std::endl;
		}

	}
}
