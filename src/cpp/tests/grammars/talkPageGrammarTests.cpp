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

#include "../../parsers/grammars/talkPageGrammar.hpp"

BOOST_AUTO_TEST_SUITE(TalkPageGrammarTests)

	std::vector<std::string> talk_page_examples = {
		": This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)",
		"[http://english.aljazeera.net/indepth/features/2011/01/2011127111226715315.html] \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE?([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC)).\n:I propose the removal of Lebanon from the list. It is  misleading to make a connection between the one-day \"protests\" (which was in response to a perceived attack on the Sunni share of the government) and the uprisings that are happening in other countries, which are the direct result of a domino effect that started from Tunisia. I will be bold and remove it from the list, unless someone will argue otherwise. [[Special:Contributions/89.83.20.127|89.83.20.127]] ([[User talk:89.83.20.127|talk]]) 14:02, 30 January 2011 (UTC)"
	};

	BOOST_DATA_TEST_CASE(test1,boost::unit_test::data::make(talk_page_examples),talk_page_str)
	{
		std::string str = talk_page_str;
		auto it = str.begin();
		boost::spirit::qi::phrase_parse(it, str.end(), WikiTalkNet::TalkPageGrammar<std::string::iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank);
		BOOST_CHECK(it == str.end());
	}

BOOST_AUTO_TEST_SUITE_END()
