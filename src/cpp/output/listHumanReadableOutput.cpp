#include "listHumanReadableOutput.h"

#include <algorithm>
#include <iomanip>
#include "dateOutput.h"

namespace Grawitas {

	const std::string HEADER_DEVIDER = "====================================";
	const std::string COMMENT_DEVIDER = "....................................";

	void listToHumanReadable(std::ostream& ostr, const ParsedTalkPage& parsed_talk_page)
	{
		for (auto section : parsed_talk_page) {
			
			// write header of the current paragraph
			ostr << HEADER_DEVIDER << std::endl;
			ostr << section.first << std::endl;
			ostr << HEADER_DEVIDER << std::endl;

			for(const auto& comment : section.second)
			{
				auto text = comment.Text;
				std::replace(text.begin(), text.end(), '\n', ' ');

				ostr
					<< std::setw(8) << comment.Id << " | "
					<< std::setw(8) << comment.ParentId << " | "
					<< std::setw(32) << comment.User << " | "
					<< std::setw(40) << to_human_readable(comment.Date) << " | "
					<< text;
				ostr << std::endl << COMMENT_DEVIDER << std::endl;
			}
		}

	}
}
