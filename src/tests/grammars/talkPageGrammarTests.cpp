#include "../../../libs/catch/catch.hpp"

#include <boost/spirit/include/qi.hpp>

#include "../../talkPageParser/grammars/talkPageGrammar.hpp"

namespace {
	std::list<Grawitas::Comment> parse_talk_page(std::string str) {
		auto it = str.cbegin();
		std::list<Grawitas::Comment> comments;
		boost::spirit::qi::phrase_parse(it, str.cend(), Grawitas::TalkPageGrammar<std::string::const_iterator, boost::spirit::qi::iso8859_1::blank_type>(), boost::spirit::qi::iso8859_1::blank, comments);

		return comments;
	}
}

TEST_CASE("talk page grammar test 1") {
	std::string str = ": This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)";
	auto comments = parse_talk_page(str);
	REQUIRE(comments.size() == 2);
}

TEST_CASE("talk page grammar test 2") {
	std::string str = "[http://english.aljazeera.net/indepth/features/2011/01/2011127111226715315.html] \"day of ragE\" albeit diffeerent reasons but was wondering if it could/should be added herE?([[User:Lihaas|Lihaas]] ([[User talk:Lihaas|talk]]) 14:28, 29 January 2011 (UTC)).\n:I propose the removal of Lebanon from the list. It is  misleading to make a connection between the one-day \"protests\" (which was in response to a perceived attack on the Sunni share of the government) and the uprisings that are happening in other countries, which are the direct result of a domino effect that started from Tunisia. I will be bold and remove it from the list, unless someone will argue otherwise. [[Special:Contributions/89.83.20.127|89.83.20.127]] ([[User talk:89.83.20.127|talk]]) 14:02, 30 January 2011 (UTC)";
	auto comments = parse_talk_page(str);
	REQUIRE(comments.size() == 2);
}
