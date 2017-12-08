#define BOOST_SPIRIT_DEBUG

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

#include "../../parsing/grammars/commentGrammar.hpp"
#include "../../output/dateOutput.h"

BOOST_AUTO_TEST_SUITE(CommentGrammarTests)

	std::vector<std::string> comment_examples = {
		": This is just a test -- [[User:Ianmckeachie|Ianmckeachie]] 04:29, 22 Oct 2004 (UTC)",
		"one can add the -arab list of self-immolations to this one, then it can also be cut off from the bouazizi article. [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)",
		"one can add the -arab list of self-immolations to this one, then it can also be cut off from the bouazizi article. [[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)",
		": one can add the selfouazizi article -- [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)",
		": This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)",
		": This is just a test --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)",
		"[http://english.aljazeera.net/indepth/features/2011/01/2011127111226715315.html] \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? ([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC))",
		"[http://english.aljazeera.net/indepth/features/2011/01/2011127111226715315.html] \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC)",
		"\"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC)",
		": This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC) sdasdjsk sjdkasd jsadkasdjas sdjaksds [[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)",
		":What, if anything, is the ''predominant'' name used by the sources to describe this series of events? [[User:Baseball Bugs|Baseball Bugs]] [[User talk:Baseball Bugs|What's up, Doc?]] [[Special:Contributions/Baseball_Bugs|carrots]] 21:22, 15 February 2011 (UTC)",
		"Previous title was \"2010–2011 <s>Middle East and North Africa</s> Arab world protests\". ←[[User:Baseball Bugs|Baseball Bugs]] ''[[User talk:Baseball Bugs|What's up, Doc?]]'' [[Special:Contributions/Baseball_Bugs|carrots]]→ 23:47, 15 February 2011 (UTC)"
	};

	std::vector<std::string> expected_users = {
		"Ianmckeachie",
		"Lihaas",
		"Lihaas",
		"Lihaas",
		"Lihaas",
		"Lihaas",
		"Lihaas",
		"Lihaas",
		"Lihaas",
		"Lihaas",
		"Baseball Bugs",
		"Baseball Bugs"
	};

	BOOST_DATA_TEST_CASE(test1,boost::unit_test::data::make(comment_examples),comment_str)
	{
		std::string str = comment_str;
		auto it = str.cbegin();
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::CommentGrammar<std::string::const_iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank);
		BOOST_CHECK(it == str.cend());
	}

	BOOST_DATA_TEST_CASE(ExtractedUsernameShouldBeCorrect,boost::unit_test::data::make(comment_examples) ^ boost::unit_test::data::make(expected_users),comment_str,expected_user)
	{
		std::string str = comment_str;
		auto it = str.cbegin();
		Grawitas::Comment parsed_comment;
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::CommentGrammar<std::string::const_iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank, parsed_comment);
		BOOST_CHECK_EQUAL(expected_user, parsed_comment.User);
	}

BOOST_AUTO_TEST_SUITE_END()
