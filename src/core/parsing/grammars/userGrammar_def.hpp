#pragma once

#include "userGrammar.hpp"

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		UserGrammar<Iterator, Skipper>::UserGrammar() : UserGrammar::base_type(start, "UserGrammar") {
			using namespace boost::spirit::qi;

			possible_user_link_labels = "[[user:", "[[user talk:", "[[user_talk:", "[[special:contributions/";
			user_related_link = no_case[possible_user_link_labels] >> username[_val = boost::spirit::_1]  >> -('|' >> +(char_  - ']')) >> "]]";
			username = +(char_ [_val += boost::spirit::_1] - '|' - ']');

			between_links_skippers = lit(' ') | '-' | '(' | ')' | '\'' | boost::spirit::qi::unicode::char_(L'→');

			start = user_related_link [_val += boost::spirit::_1] >> *between_links_skippers >> -user_related_link >> *between_links_skippers >> -user_related_link;
		}
}
