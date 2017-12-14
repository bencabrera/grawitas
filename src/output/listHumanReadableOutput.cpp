#include "listHumanReadableOutput.h"

#include <algorithm>
#include <iomanip>
#include "dateOutput.h"

namespace Grawitas {

	const std::string COMMENT_DEVIDER = "....................................";

	void listToHumanReadable(std::ostream& ostr, const std::vector<Comment>& parsed_talk_page)
	{
		for(const auto& comment : parsed_talk_page)
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
