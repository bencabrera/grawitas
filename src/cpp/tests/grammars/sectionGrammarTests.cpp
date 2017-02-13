#pragma once

#define BOOST_SPIRIT_DEBUG

#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <fstream>
#include <streambuf>
#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

#include "../../parsers/grammars/sectionGrammar.hpp"

BOOST_AUTO_TEST_SUITE(SectionGrammarTests)

	std::vector<std::string> talk_page_examples = {
		// "== Title == \n : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)\n",
		"== Title == \n : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)\n == Title2 == \n : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)\n"
	};

	BOOST_DATA_TEST_CASE(test1,boost::unit_test::data::make(talk_page_examples),talk_page_str)
	{
		std::string str = talk_page_str;
		auto it = str.begin();
		std::vector<std::pair<std::string, std::string>> result;
		boost::spirit::qi::phrase_parse(it, str.end(), WikiTalkNet::SectionGrammar<std::string::iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank, result);

		// for (auto i : result) {
			// std::cout << i.first << ":::::::" << i.second << std::endl;	
		// }
		BOOST_CHECK(it == str.end());
	}

BOOST_AUTO_TEST_SUITE_END()
