#include "../../../libs/catch/catch.hpp"

#include <boost/spirit/include/qi.hpp>

#include "../../talkPageParser/grammars/userGrammar.hpp"

namespace {
	std::string parse_user(std::string str) {
		auto it = str.cbegin();
		std::string user;
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::UserGrammar<std::string::const_iterator, boost::spirit::qi::iso8859_1::blank_type>(), boost::spirit::qi::iso8859_1::blank, user);

		return user;
	}
}

TEST_CASE("[[User:Ianmckeachie|Ianmckeachie]]") {
	REQUIRE(parse_user("[[User:Ianmckeachie|Ianmckeachie]]") == "Ianmckeachie");
}
TEST_CASE("[[User:Ianmckeachie]]") {
	REQUIRE(parse_user("[[User:Ianmckeachie]]") == "Ianmckeachie");
}
TEST_CASE("[[User:Ianmckeachie|Ianmckeachie]] ([[User talk:Ianmckeachie|talk]])") {
	REQUIRE(parse_user("[[User:Ianmckeachie|Ianmckeachie]] ([[User talk:Ianmckeachie|talk]])") == "Ianmckeachie");
}
TEST_CASE("[[User:Ianmckeachie|Ianmckeachie]] [[User talk:Ianmckeachie|talk]]") {
	REQUIRE(parse_user("[[User:Ianmckeachie|Ianmckeachie]] [[User talk:Ianmckeachie|talk]]") == "Ianmckeachie");
}
TEST_CASE("[[Special:Contributions/128.227.41.144|128.227.41.144]]") {
	REQUIRE(parse_user("[[Special:Contributions/128.227.41.144|128.227.41.144]]") == "128.227.41.144");
}
TEST_CASE("[[Special:Contributions/128.227.41.144|128.227.41.144]] ([[User talk:128.227.41.144|talk]])") {
	REQUIRE(parse_user("[[Special:Contributions/128.227.41.144|128.227.41.144]] ([[User talk:128.227.41.144|talk]])") == "128.227.41.144");
}
TEST_CASE("[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]])") {
	REQUIRE(parse_user("[[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]])") == "Lihaas");
}
TEST_CASE("[[user:thumperward|Chris Cunningham (user:thumperward: not at work)]] - [[user talk:thumperward|talk]]") {
	REQUIRE(parse_user("[[user:thumperward|Chris Cunningham (user:thumperward: not at work)]] - [[user talk:thumperward|talk]]") == "thumperward");
}
