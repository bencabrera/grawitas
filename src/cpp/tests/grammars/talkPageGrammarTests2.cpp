#pragma once

#include "../fileListDataset.hpp"

#include <fstream>
#include <streambuf>
#include <string>

#include "../../parsers/grammars/talkPageGrammar.hpp"
#include "../jsonToCommentList.h"
#include "../optimalTextMapping.h"
#include "../../models/parsedTalkPage.h"
#include "../helpers.h"
#include "../../output/dateOutput.h"

BOOST_AUTO_TEST_SUITE(FilePathTest)

	BOOST_DATA_TEST_CASE(
		test2,
		WikiTalkNet::Tests::FileListDataset("../../instances/tests/", "TALK_PAGE", ".wikisyntax")
		^ boost::unit_test::data::make(1)
		^ WikiTalkNet::Tests::FileListDataset("../../instances/tests/", "ANNOTATED", ".json"),
		talk_page_path, dummy, annotated_path
	)
	{
		using namespace WikiTalkNet;
		using namespace WikiTalkNet::Tests;

		// --- parsing the annotations ---
		std::ifstream annotationFile(annotated_path.string());
		auto annotated_comments = jsonToCommentList(annotationFile);

		// --- parsing the actual talk page wikisyntax ---
		std::ifstream talkPageFile(talk_page_path.string());
		std::string talkPageSyntax((std::istreambuf_iterator<char>(talkPageFile)), std::istreambuf_iterator<char>());
		auto it = talkPageSyntax.begin();
		std::list<Comment> parsed_comments_list;
		boost::spirit::qi::phrase_parse(it, talkPageSyntax.end(), WikiTalkNet::TalkPageGrammar<std::string::iterator, boost::spirit::qi::blank_type>(), boost::spirit::qi::blank, parsed_comments_list);

		// --- compute optimal mapping from annotations to parsed comments ---
		std::vector<Comment> parsed_comments(parsed_comments_list.begin(), parsed_comments_list.end());
		auto res = computeOptimalTextMapping(annotated_comments, parsed_comments);

		BOOST_CHECK_EQUAL(0, std::get<1>(res).size());
		
		for(auto& match : std::get<0>(res))
		{
			auto annoIdx = match.first;
			auto parserIdx = match.second;

			BOOST_CHECK_EQUAL(annotated_comments[annoIdx].User, parsed_comments[parserIdx].User);
		}

		// std::cout << "Summary: " << std::endl;
		// for(auto& c : parsed_comments)
		// {
			// std::cout << "---------------------------------------------------------" << std::endl;
			// std::cout << c << std::endl;
			// std::cout << "---------------------------------------------------------" << std::endl;
		// }
		// std::cout << "Found matches: " << std::endl;
		// for (auto el : std::get<0>(res)) {
			// std::cout << el.first << " " << el.second << std::endl;
		// }
		// std::cout << "Mismatched: " << *std::get<1>(res).begin() << std::endl;
		// std::cout << "----" << std::endl << parsed_comments[0].Text << "'" << std::endl << "-----" << std::endl << annotated_comments[0].Text << "'" << std::endl << "---" << std::endl;

		// BOOST_CHECK(talk_page_path == annotated_path);
	}

BOOST_AUTO_TEST_SUITE_END()
