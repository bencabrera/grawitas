#include "listCsvOutput.h"

#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include "dateOutput.h"

namespace Grawitas {

	std::string csv_encode(const std::string& str)
	{
		auto rtn = boost::replace_all_copy(str, "\"", "\"\"");
		boost::replace_all(rtn, "\n", "\\n");
		return rtn;
	}

	void listToCsv(std::ostream& ostr, const ParsedTalkPage& parsed_talk_page)
	{
		constexpr char CSV_DELIMITER = ',';

		ostr
			<< "Comment Id" << CSV_DELIMITER
			<< "Parent Id" << CSV_DELIMITER
			<< "User" << CSV_DELIMITER
			<< "Date" << CSV_DELIMITER
			<< "Text" << std::endl;

		for (auto section : parsed_talk_page) {
			for(const auto& comment : section.second)
			{
				ostr
					<< comment.Id << CSV_DELIMITER
					<< comment.ParentId << CSV_DELIMITER
					<< "\"" << csv_encode(comment.User) << "\"" << CSV_DELIMITER
					<< "\"" << to_iso_8601(comment.Date) << "\"" << CSV_DELIMITER
					<< "\"" << csv_encode(comment.Text) << "\"" << std::endl;
			}
		}
	}
}
