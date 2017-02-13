#pragma once
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <fstream>
#include <streambuf>
#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_repeat.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>

#include "../../parsers/grammars/userGrammar.hpp"

BOOST_AUTO_TEST_SUITE(UserGrammarTests)

	std::vector<std::string> user_examples = {
		"[[User:Ianmckeachie|Ianmckeachie]]",
		"[[User:Ianmckeachie]]",
		"[[User:Ianmckeachie|Ianmckeachie]] ([[User talk:Ianmckeachie|talk]])",
		"[[User:Ianmckeachie|Ianmckeachie]] [[User talk:Ianmckeachie|talk]]",
		"[[Special:Contributions/128.227.41.144|128.227.41.144]]",
		"[[Special:Contributions/128.227.41.144|128.227.41.144]] ([[User talk:128.227.41.144|talk]])",
		"[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]])",
		"[[user:thumperward|Chris Cunningham (user:thumperward: not at work)]] - [[user talk:thumperward|talk]]"
	};

	std::vector<std::string> expected_values = {
		"Ianmckeachie",
		"Ianmckeachie",
		"Ianmckeachie",
		"Ianmckeachie",
		"128.227.41.144",
		"128.227.41.144",
		"Lihaas",
		"thumperward"
	};

	BOOST_DATA_TEST_CASE(ShouldBeAbleToParse,boost::unit_test::data::make(user_examples),user_str)
	{
		std::string str = user_str;
		auto it = str.begin();
		boost::spirit::qi::phrase_parse(it, str.end(), WikiTalkNet::UserGrammar<std::string::iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank);
		BOOST_CHECK(it == str.end());
	}


	BOOST_DATA_TEST_CASE(ExtractedUsernameShouldBeCorrect,boost::unit_test::data::make(user_examples) ^ boost::unit_test::data::make(expected_values),user_str,expected)
	{
		std::string str = user_str;
		auto it = str.begin();
		std::string parsedUsername;
		boost::spirit::qi::phrase_parse(it, str.end(), WikiTalkNet::UserGrammar<std::string::iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank, parsedUsername);
		BOOST_CHECK(it == str.end());
		BOOST_CHECK_EQUAL(expected, parsedUsername);
	}

	// BOOST_DATA_TEST_CASE(ExtractedUsernameShouldBeCorrect2,boost::unit_test::data::make(user_examples) ^ boost::unit_test::data::make(expected_values),user_str,expected)
	// {
		// std::string str = user_str;
		// auto it = str.begin();
		// std::string parsedUsername;
		// boost::spirit::qi::phrase_parse(it, str.end(), WikiTalkNet::UserGrammar<std::string::iterator, boost::spirit::qi::blank_type>().username_link, boost::spirit::qi::blank, parsedUsername);
		// BOOST_CHECK(it == str.end());
		// BOOST_CHECK_EQUAL(expected, parsedUsername);
	// }

BOOST_AUTO_TEST_SUITE_END()
