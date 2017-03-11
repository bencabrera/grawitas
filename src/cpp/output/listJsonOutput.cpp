#include "listCsvOutput.h"

#include <algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "dateOutput.h"

namespace Grawitas {

	void listToJson(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage)
	{
		namespace pt = boost::property_tree;

		pt::ptree comments;
		std::size_t commentIdOffset = 0;
		for (auto paragraph : parsedTalkPage) {
			for(auto it = paragraph.second.begin(); it != paragraph.second.end(); ++it)
			{
				auto comment = *it;
				auto text = comment.Text;
				std::replace( text.begin(), text.end(), ';', '.');

				pt::ptree commentNode;
				commentNode.put("id",comment.Id);
				commentNode.put("parent_id",comment.ParentId);
				commentNode.put("user",comment.User);
				std::stringstream ss;
				ss << to_iso_8601(comment.Date);
				commentNode.put("date",ss.str());
				commentNode.put("level",comment.IndentLevel);
				commentNode.put("text",comment.Text);

				comments.push_back(std::make_pair("",commentNode));
			}
			commentIdOffset += paragraph.second.size();
		}

		pt::ptree root;
		root.add_child("comments", comments);

		pt::write_json(ostr, root);
	}
}
