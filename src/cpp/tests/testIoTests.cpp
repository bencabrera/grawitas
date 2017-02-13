#pragma once
#include <boost/test/included/unit_test.hpp>

#include <streambuf>

#include "jsonToCommentList.h"

BOOST_AUTO_TEST_SUITE(LoadingTests)

BOOST_AUTO_TEST_CASE(JsonAnnotationLoaderShouldWork)
{
	std::string annotatedPath("../instances/Vampire:_The_Masquerade_–_Bloodlines_ANNOTATED.json");
	std::ifstream inputFile(annotatedPath);
	auto annotated_comments = WikiTalkNet::Tests::jsonToCommentList(inputFile);
	BOOST_CHECK_EQUAL(annotated_comments.size(), 10);

	BOOST_CHECK_EQUAL(annotated_comments[3].User, "Alkanoonion");
	BOOST_CHECK_EQUAL(annotated_comments[4].User, "Chris Cunningham");
	BOOST_CHECK_EQUAL(annotated_comments[0].Id, 1);
	BOOST_CHECK_EQUAL(annotated_comments[1].Id, 2);
	BOOST_CHECK_EQUAL(annotated_comments[1].ParentId, 1);
	BOOST_CHECK_EQUAL(annotated_comments[2].Id, 3);
}

BOOST_AUTO_TEST_SUITE_END()
