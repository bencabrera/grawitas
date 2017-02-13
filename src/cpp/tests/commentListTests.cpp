#pragma once
#include <boost/test/included/unit_test.hpp>

#include <fstream>
#include <streambuf>
#include <string>

#include "../parsers/cleaningSyntax.h"
#include "../parsers/commentParser.h"
#include "../parsers/talkPageParser.h"
#include "../models/graph.h"
#include "../graphComputation/graphComputationCache.h"
#include "../models/parsedTalkPage.h"
#include "jsonToCommentList.h"
#include "optimalTextMapping.h"

BOOST_AUTO_TEST_SUITE(CommentListTests)

BOOST_AUTO_TEST_CASE(Example1)
{
	using namespace WikiTalkNet;
	using namespace WikiTalkNet::Tests;

	std::string talkPagePath("../instances/Vampire:_The_Masquerade_–_Bloodlines_TALK_PAGE.wikisyntax");
	std::string annotatedPath("../instances/Vampire:_The_Masquerade_–_Bloodlines_ANNOTATED.json");

	std::ifstream talkPageFile(talkPagePath);
	std::ifstream annotationFile(annotatedPath);


    std::string wikiSyntaxContent = "\n";
	wikiSyntaxContent.append(std::istreambuf_iterator<char>(talkPageFile), std::istreambuf_iterator<char>());

	// --- parsing the actual talk page ---
    // first split the syntax into paragraphs with their respective titles
    ParagraphWithHeader p({ HeaderDescriptor(), wikiSyntaxContent });
    auto paragraphs = std::list<ParagraphWithHeader>{ p };
    splitByHeader(paragraphs, 2);
    splitByHeader(paragraphs, 3);

    // split the paragraphs into comments, parse them and calculate their ids
	ParsedTalkPage parsedTalkPage;
    for(auto& paragraph : paragraphs)
    {
    	unsigned commentId = 1;
        auto list = parseLinesIntoComments(splitIntoComments(paragraph.second));
        calculateIds(list,commentId);
		parsedTalkPage.push_back({ paragraph.first, list });
    }


	// --- parsing the annotations ---
	auto annotated_comments = WikiTalkNet::Tests::jsonToCommentList(annotationFile);


	// --- compute optimal mapping from annotations to parsed comments ---
	auto parsed_comments_list = flattenParsedTalkPage(parsedTalkPage);
	std::vector<Comment> parsed_comments(parsed_comments_list.begin(), parsed_comments_list.end());
	auto res = computeOptimalTextMapping(annotated_comments, parsed_comments);


	BOOST_CHECK_EQUAL(std::get<1>(res).size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
