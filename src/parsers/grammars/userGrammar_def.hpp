#pragma once

#include "userGrammar.hpp"

namespace WikiTalkNet {

	template<typename Iterator, typename Skipper>
		UserGrammar<Iterator, Skipper>::UserGrammar() : UserGrammar::base_type(start, "UserGrammar") {
			// namespace qi = boost::spirit::qi;
			using namespace boost::spirit::qi;

			user_related_link = (no_case["[[User:"] | no_case["[[User talk:"] | no_case["[[Special:Contributions/"]) >> +((char_ [_val += boost::spirit::_1] - '|' - ']') )  >> -('|' >> +(char_  - ']')) >> "]]";

			// username_link = no_case["[[User:"] >> +((char_ [_val += boost::spirit::_1] - '|' - ']') )  >> -('|' >> +(char_  - ']')) >> "]]";	
			// ip_adress = +(char_('0','9') | char_('.'))[_val += boost::spirit::_1];
			// ip_adress_username_link = no_case["[[Special:Contributions/"] >> ip_adress [_val += boost::spirit::_1] >> -('|' >> ip_adress) >> "]]";	
			// talk_link = -char_('(') >> no_case["[[User talk:"] >> +(char_ - '|' - ']') >> -(eps >> '|' >> no_case["talk"] ) >> "]]" >> -char_(')');	

			between_links_skippers = lit(' ') | '-' | '(' | ')' | '\'' | boost::spirit::qi::unicode::char_(L'→');

			start = user_related_link [_val += boost::spirit::_1] >> *between_links_skippers >> -user_related_link >> *between_links_skippers >> -user_related_link;
			// start = (username_link [_val += boost::spirit::_1] | ip_adress_username_link [_val = boost::spirit::_1]) [_val = boost::spirit::_1] >> *(char_(' ') | char_('-')) >> -talk_link;

			start.name("UserGrammar::Start");
			username_link.name("UserGrammar::UsernameLink");
			ip_adress_username_link.name("UserGrammar::IpAdressUsernameLink");
			ip_adress.name("UserGrammar::IpAdress");
			talk_link.name("UserGrammar::TalkLink");


			// on_error<fail>(start, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(username_link, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(ip_adress_username_link, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(ip_adress, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(talk_link, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);

		}
}
