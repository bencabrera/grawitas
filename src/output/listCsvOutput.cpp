#include "listCsvOutput.h"

#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include "dateOutput.h"

namespace {

	std::string csv_encode(const std::string& str)
	{
		auto rtn = boost::replace_all_copy(str, "\"", "\"\"");
		boost::replace_all(rtn, "\n", "\\n");
		return rtn;
	}

	void write_row_to_csv(std::ostream& ostr, const std::vector<std::string>& row)
	{
		static const std::string CSV_DELIMITER = ",";

		bool first = true;
		for(const auto& col : row)
		{
			ostr << ((first) ? "" : CSV_DELIMITER) << col;
			first = false;
		}
	}
}

namespace Grawitas {

	void listToCsv(std::ostream& ostr, const std::vector<Comment>& parsed_talk_page, const std::set<std::string> included_fields)
	{
		std::vector<std::string> header_row;
		if(included_fields.count("id"))
			header_row.push_back("\"id\"");

		if(included_fields.count("parent_id"))
			header_row.push_back("\"parent_id\"");

		if(included_fields.count("user"))
			header_row.push_back("\"user\"");

		if(included_fields.count("date"))
			header_row.push_back("\"date\"");

		if(included_fields.count("section"))
			header_row.push_back("\"section\"");

		if(included_fields.count("article"))
			header_row.push_back("\"article\"");

		if(included_fields.count("text"))
			header_row.push_back("\"text\"");

		write_row_to_csv(ostr, header_row);
		ostr << std::endl;

		
		for(const auto& comment : parsed_talk_page)
		{
			std::vector<std::string> row;
			if(included_fields.count("id"))
				row.push_back(std::to_string(comment.Id));

			if(included_fields.count("parent_id"))
				row.push_back(std::to_string(comment.ParentId));

			if(included_fields.count("user"))
				row.push_back("\"" + csv_encode(comment.User) + "\"");

			if(included_fields.count("date"))
				row.push_back("\"" + to_iso_8601(comment.Date) + "\"");

			if(included_fields.count("section"))
				row.push_back("\"" + csv_encode(comment.Section) + "\"");

			if(included_fields.count("article"))
				row.push_back("\"" + csv_encode(comment.Article) + "\"");

			if(included_fields.count("text"))
				row.push_back("\"" + csv_encode(comment.Text) + "\"");

			write_row_to_csv(ostr, header_row);
			ostr << std::endl;
		}
	}
}
