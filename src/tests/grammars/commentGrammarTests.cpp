#include "../../../libs/catch/catch.hpp"

#include <fstream>
#include <streambuf>
#include <string>

#include <boost/spirit/include/qi.hpp>

#include "../../talkPageParser/grammars/commentGrammar.hpp"
#include "../../output/dateOutput.h"

namespace {
	Grawitas::Comment parse_comment(std::string str) 
	{
		auto it = str.cbegin();
		Grawitas::Comment parsed_comment;
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::CommentGrammar<std::string::const_iterator, boost::spirit::qi::iso8859_1::blank_type>(), boost::spirit::qi::iso8859_1::blank, parsed_comment);

		return parsed_comment;
	}
}

TEST_CASE("USER - : This is just a test -- [[User:Ianmckeachie|Ianmckeachie]] 04:29, 22 Oct 2004 (UTC)") {
	auto comment = parse_comment(": This is just a test -- [[User:Ianmckeachie|Ianmckeachie]] 04:29, 22 Oct 2004 (UTC)");
	REQUIRE(comment.User == "Ianmckeachie");
}
TEST_CASE("USER - one can add the -arab list of self-immolations to this one, then it can also be cut off from the bouazizi article. [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)") {
	auto comment = parse_comment("one can add the -arab list of self-immolations to this one, then it can also be cut off from the bouazizi article. [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)");
	REQUIRE(comment.User == "Lihaas");
}

TEST_CASE("USER - one can add the -arab list of self-immolations to this one, then it can also be cut off from the bouazizi article. [[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)") {
	auto comment = parse_comment("one can add the -arab list of self-immolations to this one, then it can also be cut off from the bouazizi article. [[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)");
	REQUIRE(comment.User == "Lihaas");
}
TEST_CASE("USER - : one can add the selfouazizi article -- [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)") {
	auto comment = parse_comment(": one can add the selfouazizi article -- [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)");
	REQUIRE(comment.User == "Lihaas");
}
TEST_CASE("USER - : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)") {
	auto comment = parse_comment(": This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)");
	REQUIRE(comment.User == "Lihaas");
}
TEST_CASE("USER - : This is just a test --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)") {
	auto comment = parse_comment(": This is just a test --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)");
	REQUIRE(comment.User == "Lihaas");
}
TEST_CASE("USER - [http://english.aljazeera.net/indepth/features/2011/01/2011127111226715315.html] \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? ([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC))") {
	auto comment = parse_comment("[http://english.aljazeera.net/indepth/features/2011/01/2011127111226715315.html] \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? ([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC))");
	REQUIRE(comment.User == "Lihaas");
}
TEST_CASE("USER - [http://english.aljazeera.net/indepth/features/2011/01/2011127111226715315.html] \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC)") {
	auto comment = parse_comment("[http://english.aljazeera.net/indepth/features/2011/01/2011127111226715315.html] \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC)");
	REQUIRE(comment.User == "Lihaas");
}
TEST_CASE("USER - \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC)") {
	auto comment = parse_comment("\"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE? [[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC)");
	REQUIRE(comment.User == "Lihaas");
}
TEST_CASE("USER - : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC) sdasdjsk sjdkasd jsadkasdjas sdjaksds [[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)") {
	auto comment = parse_comment(": This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC) sdasdjsk sjdkasd jsadkasdjas sdjaksds [[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)");
	REQUIRE(comment.User == "Lihaas");
}
TEST_CASE("USER - :What, if anything, is the ''predominant'' name used by the sources to describe this series of events? [[User:Baseball Bugs|Baseball Bugs]] [[User talk:Baseball Bugs|What's up, Doc?]] [[Special:Contributions/Baseball_Bugs|carrots]] 21:22, 15 February 2011 (UTC)") {
	auto comment = parse_comment(":What, if anything, is the ''predominant'' name used by the sources to describe this series of events? [[User:Baseball Bugs|Baseball Bugs]] [[User talk:Baseball Bugs|What's up, Doc?]] [[Special:Contributions/Baseball_Bugs|carrots]] 21:22, 15 February 2011 (UTC)");
	REQUIRE(comment.User == "Baseball Bugs");
}
TEST_CASE("USER - Previous title was \"2010–2011 <s>Middle East and North Africa</s> Arab world protests\". ←[[User:Baseball Bugs|Baseball Bugs]] ''[[User talk:Baseball Bugs|What's up, Doc?]]'' [[Special:Contributions/Baseball_Bugs|carrots]]→ 23:47, 15 February 2011 (UTC)") {
	auto comment = parse_comment("Previous title was \"2010–2011 <s>Middle East and North Africa</s> Arab world protests\". ←[[User:Baseball Bugs|Baseball Bugs]] ''[[User talk:Baseball Bugs|What's up, Doc?]]'' [[Special:Contributions/Baseball_Bugs|carrots]]→ 23:47, 15 February 2011 (UTC)");
	REQUIRE(comment.User == "Baseball Bugs");
}
