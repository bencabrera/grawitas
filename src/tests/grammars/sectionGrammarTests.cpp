#include "../../../libs/catch/catch.hpp"

#include "../../talkPageParser/parsing.h"

TEST_CASE("SectionGrammarTest1") {
	std::string talk_page_str = "==Title== \n : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)\n ==Title2== \n : This is just a test --[[User:Lihaas|Lihaas]] 09:37, 29 January 2011 (UTC)\n: This is just a test2 --[[User:Lihaas|Lihaas]]   09:37, 29 January 2011 (UTC)\n";

	auto sections = Grawitas::split_into_sections(talk_page_str);

	REQUIRE(sections.size() == 2);
	REQUIRE(std::get<0>(sections[0]) == "Title");
	REQUIRE(std::get<0>(sections[1]) == "Title2");
}
