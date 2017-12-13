#include "../../../libs/catch/catch.hpp"

#include <fstream>
#include <streambuf>
#include <string>

#include <boost/spirit/include/qi.hpp>

#include "../../talkPageParser/grammars/dateGrammar.hpp"

namespace {
	std::tm parse_date(std::string str) 
	{
		auto it = str.cbegin();
		std::tm parsed_date{};
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::DateGrammar<std::string::const_iterator, boost::spirit::qi::iso8859_1::blank_type>(), boost::spirit::qi::iso8859_1::blank, parsed_date);

		return parsed_date;
	}

	bool operator==(const std::tm t1, const std::tm t2) {
		return (t1.tm_mday == t2.tm_mday && t1.tm_min == t2.tm_min && t1.tm_mon == t2.tm_mon && t1.tm_year == t2.tm_year && t1.tm_hour == t2.tm_hour && t1.tm_sec == t2.tm_sec);
	}
}

TEST_CASE("04:29, 22 Oct 2004 (UTC)") {
	REQUIRE(parse_date("04:29, 22 Oct 2004 (UTC)") == std::tm{0,29,4,22,9,104,0,0,0,0,0});
}
TEST_CASE("06:06, Nov 1, 2004 (UTC)") {
	REQUIRE(parse_date("06:06, Nov 1, 2004 (UTC)") == std::tm{0,6,6,1,10,104,0,0,0,0,0});
}
TEST_CASE("15:33, 2004 Nov 13 (UTC)") {
	REQUIRE(parse_date("15:33, 2004 Nov 13 (UTC)") == std::tm{0,33,15,13,10,104,0,0,0,0,0});
}
TEST_CASE("11:57, 9 October 2015 (UTC)") {
	REQUIRE(parse_date("11:57, 9 October 2015 (UTC)") == std::tm{0,57,11,9,9,115,0,0,0,0,0});
}
TEST_CASE("00:00, 16 February 2011 (UTC)") {
	REQUIRE(parse_date("00:00, 16 February 2011 (UTC)") == std::tm{0,0,0,16,1,111,0,0,0,0,0});
}
TEST_CASE("09:37, 29 January 2011 (UTC)") {
	REQUIRE(parse_date("09:37, 29 January 2011 (UTC)") == std::tm{0,37,9,29,0,111,0,0,0,0,0});
}
