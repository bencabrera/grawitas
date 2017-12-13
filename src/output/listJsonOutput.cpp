#include "listJsonOutput.h"

#include <algorithm>

#include "../../libs/json/json.hpp"
#include "dateOutput.h"

namespace Grawitas {

	void listToJson(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage, const std::set<std::string> included_fields, const bool beautify)
	{
		using json = nlohmann::json;

		json json_comments;
		std::size_t commentIdOffset = 0;
		for (const auto& paragraph : parsedTalkPage) {
			for(const auto& c : paragraph.second) 
			{
				json json_node;

				if(included_fields.count("id"))
					json_node["id"] = c.Id;

				if(included_fields.count("parent_id"))
					json_node["parent_id"] = c.ParentId;

				if(included_fields.count("user"))
					json_node["user"] = c.User;

				if(included_fields.count("date"))
					json_node["date"] = to_iso_8601(c.Date);

				if(included_fields.count("level"))
					json_node["level"] = c.IndentLevel;

				if(included_fields.count("article"))
					json_node["article"] = c.Article;

				if(included_fields.count("text")) {
					auto text = c.Text;
					std::replace(text.begin(), text.end(), ';', '.');
					json_node["text"] = c.Text;
				}

				json_comments.push_back(json_node);
			}
			commentIdOffset += paragraph.second.size();
		}

		if(beautify)
			ostr << std::setw(4) << json_comments;
		else
			ostr << json_comments;
	}
}
