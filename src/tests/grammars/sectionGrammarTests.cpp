#define BOOST_SPIRIT_DEBUG

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <fstream>
#include <streambuf>
#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

#include "../../parsing/grammars/sections/sectionGrammar.hpp"

BOOST_AUTO_TEST_SUITE(SectionGrammarTests)

	std::vector<std::string> talk_page_examples = {
		"==Title== \n : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)\n ==Title2== \n : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)\n"
	};

	BOOST_DATA_TEST_CASE(sections_should_be_split_correctly,boost::unit_test::data::make(talk_page_examples),talk_page_str)
	{
		std::string str = talk_page_str;
		auto it = str.cbegin();
		std::vector<std::tuple<std::string, std::string>> sections;
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::SectionGrammar<std::string::const_iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank, sections);

		// remove empty sections
		sections.erase(std::remove_if(sections.begin(), sections.end(), [](const std::tuple<std::string, std::string>& t) {
			return std::get<1>(t).empty();
		}), sections.end());

		BOOST_CHECK_EQUAL(2, sections.size());
		BOOST_CHECK_EQUAL("Title", std::get<0>(sections[0]));
		BOOST_CHECK_EQUAL("Title2", std::get<0>(sections[1]));
	}

BOOST_AUTO_TEST_SUITE_END()
