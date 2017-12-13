#include "../../../libs/catch/catch.hpp"

#include <fstream>
#include <streambuf>
#include <string>

#include <boost/spirit/include/qi.hpp>

#include "../../talkPageParser/grammars/signatureGrammar.hpp"

namespace {
	bool operator==(const std::tm t1, const std::tm t2) {
		return (t1.tm_mday == t2.tm_mday && t1.tm_min == t2.tm_min && t1.tm_mon == t2.tm_mon && t1.tm_year == t2.tm_year && t1.tm_hour == t2.tm_hour && t1.tm_sec == t2.tm_sec);
	}

	std::pair<std::string, std::tm> parse_signature(std::string str) 
	{
		auto it = str.cbegin();
		std::pair<std::string, std::tm> parsed_signature;
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::SignatureGrammar<std::string::const_iterator, boost::spirit::qi::iso8859_1::blank_type>(), boost::spirit::qi::iso8859_1::blank, parsed_signature);

		return parsed_signature;
	}
}

TEST_CASE("[[User:Ianmckeachie|Ianmckeachie]] 04:29, 22 Oct 2004 (UTC)") {
	auto signature = parse_signature("[[User:Ianmckeachie|Ianmckeachie]] 04:29, 22 Oct 2004 (UTC)");
	REQUIRE(signature.first == "Ianmckeachie");
	REQUIRE(signature.second == std::tm{0,29,4,22,9,104,0,0,0,0,0});
}
TEST_CASE("[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)") {
	auto signature = parse_signature("[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)");
	REQUIRE(signature.first == "Lihaas");
	REQUIRE(signature.second == std::tm{0,37,9,29,0,111,0,0,0,0,0});
}
TEST_CASE("([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC))") {
	auto signature = parse_signature("[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 09:37, 29 January 2011 (UTC)");
	REQUIRE(signature.first == "Lihaas");
	REQUIRE(signature.second == std::tm{0,37,9,29,0,111,0,0,0,0,0});
}
TEST_CASE("([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]])         09:37, 29 January 2011 (UTC))") {
	auto signature = parse_signature("([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]])         09:37, 29 January 2011 (UTC))");
	REQUIRE(signature.first == "Lihaas");
	REQUIRE(signature.second == std::tm{0,37,9,29,0,111,0,0,0,0,0});
}
TEST_CASE("[[user:thumperward|Chris Cunningham (user:thumperward: not at work)]] - [[user talk:thumperward|talk]] 11:02, 25 August 2010 (UTC)") {
	auto signature = parse_signature("[[user:thumperward|Chris Cunningham (user:thumperward: not at work)]] - [[user talk:thumperward|talk]] 11:02, 25 August 2010 (UTC)");
	REQUIRE(signature.first == "thumperward");
	REQUIRE(signature.second == std::tm{0,2,11,25,7,110,0,0,0,0,0});
}
TEST_CASE("[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|Talk]]) 02:14, 16 February 2011 (UTC)") {
	auto signature = parse_signature("[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|Talk]]) 02:14, 16 February 2011 (UTC)");
	REQUIRE(signature.first == "Lihaas");
	REQUIRE(signature.second == std::tm{0,14,2,16,1,111,0,0,0,0,0});
}
TEST_CASE("[[User:Master&amp;Expert|'''Master&amp;'''Expert]] ([[User talk:Master&amp;Expert|Talk]]) 02:14, 16 February 2011 (UTC)") {
	auto signature = parse_signature("[[User:Master&amp;Expert|'''Master&amp;'''Expert]] ([[User talk:Master&amp;Expert|Talk]]) 02:14, 16 February 2011 (UTC)");
	REQUIRE(signature.first == "Master&amp;Expert");
	REQUIRE(signature.second == std::tm{0,14,2,16,1,111,0,0,0,0,0});
}
