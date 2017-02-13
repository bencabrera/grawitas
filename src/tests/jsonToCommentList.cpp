#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "jsonToCommentList.h"

namespace WikiTalkNet {
	namespace Tests {

		std::vector<WikiTalkNet::Comment> jsonToCommentList (std::istream& istr)
		{
			namespace pt = boost::property_tree;

			std::vector<WikiTalkNet::Comment> rtn;

			pt::ptree root;
			pt::read_json(istr, root);	

			for (pt::ptree::value_type& annotated_comment : root.get_child("annotated_comments"))
			{
				WikiTalkNet::Comment c;
				
				c.Id = std::stoi(annotated_comment.second.get_child("id").data());
				c.ParentId = std::stoi(annotated_comment.second.get_child("parent_id").data());
				c.User = annotated_comment.second.get_child("user").data();
				// TODO
				// c.Date = annotated_comment.second.get_child("date").data();
				c.Text = annotated_comment.second.get_child("text").data();

				rtn.push_back(c);
			}

			return rtn;
		}

	} 
}
