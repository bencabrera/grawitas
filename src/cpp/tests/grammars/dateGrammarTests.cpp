#include <boost/test/unit_test.hpp>
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

#include "../../parsing/grammars/dateGrammar.hpp"
#include "../helpers.h"
#include "../../output/dateOutput.h"

BOOST_AUTO_TEST_SUITE(DateGrammarTests)

	std::vector<std::string> date_examples = {
		"04:29, 22 Oct 2004 (UTC)",
		"06:06, Nov 1, 2004 (UTC)",
		"15:33, 2004 Nov 13 (UTC)",
		"11:57, 9 October 2015 (UTC)",
		"00:00, 16 February 2011 (UTC)",
		"09:37, 29 January 2011 (UTC)"
	};

	std::vector<std::tm> expected_dates = {
		// tm_sec	tm_min	tm_hour	tm_mday	tm_mon	tm_year	tm_wday	tm_yday	tm_isdst
		{	   0,	   29,		 4,		22,		9,	  104 							},
		{	   0,	    6,		 6,		 1,	   10,	  104 							},
		{	   0,	   33,		15,		13,	   10,	  104 							},
		{	   0,	   57,		11,		 9,		9,	  115 							},
		{	   0,	    0,		 0,		16,		1,	  111 							},
		{	   0,	   37,		 9,		29,		0,	  111 							},
	};

	BOOST_DATA_TEST_CASE(should_run,boost::unit_test::data::make(date_examples),date_str)
	{
		std::string str = date_str;
		auto it = str.cbegin();
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::DateGrammar<std::string::const_iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank);
		BOOST_CHECK(it == str.cend());
	}

	BOOST_DATA_TEST_CASE(extracted,boost::unit_test::data::make(date_examples) ^ boost::unit_test::data::make(expected_dates), date_str,expected_date)
	{
		std::string str = date_str;
		auto it = str.cbegin();
		std::tm parsed_date{};
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::DateGrammar<std::string::const_iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank, parsed_date);
		BOOST_CHECK_EQUAL(expected_date, parsed_date);
	}

BOOST_AUTO_TEST_SUITE_END()
