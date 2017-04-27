#define BOOST_SPIRIT_DEBUG

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

#include "../../parsing/grammars/signatureGrammar.hpp"
#include "../../output/dateOutput.h"
#include "../helpers.h"

BOOST_AUTO_TEST_SUITE(SignatureGrammarTests)

	std::vector<std::string> signature_examples = {
		"[[User:Ianmckeachie|Ianmckeachie]] 04:29, 22 Oct 2004 (UTC)",
		"[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)",
		"([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC))",
		"([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]])         09:37, 29 January 2011 (UTC))",
		"[[user:thumperward|Chris Cunningham (user:thumperward: not at work)]] - [[user talk:thumperward|talk]] 11:02, 25 August 2010 (UTC)",
		"[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|Talk]]) 02:14, 16 February 2011 (UTC)",
		"[[User:Master&amp;Expert|'''Master&amp;'''Expert]] ([[User talk:Master&amp;Expert|Talk]]) 02:14, 16 February 2011 (UTC)"
	};

	std::vector<std::string> expected_usernames = {
		"Ianmckeachie", 
		"Lihaas",
		"Lihaas",
		"Lihaas",
		"thumperward",
		"Lihaas",
		"Master&amp;Expert"
	};

	std::vector<std::tm> expected_dates = {
		// tm_sec	tm_min	tm_hour	tm_mday	tm_mon	tm_year	tm_wday	tm_yday	tm_isdst
		{	   0,	   29,		 4,		22,		9,	  104 							},
		{	   0,	   37,		 9,		29,		0,	  111 							},
		{	   0,	   37,		 9,		29,		0,	  111 							},
		{	   0,	   37,		 9,		29,		0,	  111 							},
		{	   0,	    2,		11,		25,		7,	  110 							},
		{	   0,	   14,		 2,		16,		1,	  111 							},
		{	   0,	   14,		 2,		16,		1,	  111 							}
	};

	BOOST_DATA_TEST_CASE(should_run,boost::unit_test::data::make(signature_examples),signature_str)
	{
		std::string str = signature_str;
		auto it = str.cbegin();
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::SignatureGrammar<std::string::const_iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank);
		BOOST_CHECK(it == str.cend());
	}

	BOOST_DATA_TEST_CASE(extracted,boost::unit_test::data::make(signature_examples) ^ boost::unit_test::data::make(expected_usernames) ^ boost::unit_test::data::make(expected_dates),signature_str,expected_user, expected_date)
	{
		std::string str = signature_str;
		auto it = str.cbegin();
		std::pair<std::string, std::tm> parsed_signature;
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::SignatureGrammar<std::string::const_iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank, parsed_signature);
		BOOST_CHECK_EQUAL(expected_user, parsed_signature.first);
		BOOST_CHECK_EQUAL(expected_date, parsed_signature.second);
	}

BOOST_AUTO_TEST_SUITE_END()
