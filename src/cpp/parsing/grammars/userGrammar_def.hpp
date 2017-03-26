#pragma once

#include "userGrammar.hpp"

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		UserGrammar<Iterator, Skipper>::UserGrammar() : UserGrammar::base_type(start, "UserGrammar") {
			using namespace boost::spirit::qi;

			possible_user_link_labels = "[[user:", "[[user talk:", "[[special:contributions/";
			user_related_link = no_case[possible_user_link_labels] >> +((char_ [_val += boost::spirit::_1] - '|' - ']') )  >> -('|' >> +(char_  - ']')) >> "]]";

			between_links_skippers = lit(' ') | '-' | '(' | ')' | '\'' | boost::spirit::qi::unicode::char_(L'→');

			start = user_related_link [_val += boost::spirit::_1] >> *between_links_skippers >> -user_related_link >> *between_links_skippers >> -user_related_link;

			// start.name("UserGrammar::Start");


			// on_error<fail>(start, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(username_link, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(ip_adress_username_link, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(ip_adress, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(talk_link, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);

		}
}
