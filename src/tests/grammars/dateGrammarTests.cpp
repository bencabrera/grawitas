#include "../../../libs/catch/catch.hpp"

#include <fstream>
#include <streambuf>
#include <string>

#include <boost/spirit/include/qi.hpp>

#include "../../talkPageParser/grammars/dateGrammar.hpp"
#include "../../talkPageParser/date.h"

namespace {
	Grawitas::Date parse_date(std::string str) 
	{
		auto it = str.cbegin();
		Grawitas::Date parsed_date{};
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::DateGrammar<std::string::const_iterator, boost::spirit::qi::iso8859_1::blank_type>(), boost::spirit::qi::iso8859_1::blank, parsed_date);

		return parsed_date;
	}
}

TEST_CASE("04:29, 22 Oct 2004 (UTC)") {
	REQUIRE(parse_date("04:29, 22 Oct 2004 (UTC)") == Grawitas::make_date(0,29,4,22,10,2004));
}
TEST_CASE("06:06, Nov 1, 2004 (UTC)") {
	REQUIRE(parse_date("06:06, Nov 1, 2004 (UTC)") == Grawitas::make_date(0,6,6,1,11,2004));
}
TEST_CASE("15:33, 2004 Nov 13 (UTC)") {
	REQUIRE(parse_date("15:33, 2004 Nov 13 (UTC)") == Grawitas::make_date(0,33,15,13,11,2004));
}
TEST_CASE("11:57, 9 October 2015 (UTC)") {
	REQUIRE(parse_date("11:57, 9 October 2015 (UTC)") == Grawitas::make_date(0,57,11,9,10,2015));
}
TEST_CASE("00:00, 16 February 2011 (UTC)") {
	REQUIRE(parse_date("00:00, 16 February 2011 (UTC)") == Grawitas::make_date(0,0,0,16,2,2011));
}
TEST_CASE("09:37, 29 January 2011 (UTC)") {
	REQUIRE(parse_date("09:37, 29 January 2011 (UTC)") == Grawitas::make_date(0,37,9,29,1,2011));
}
