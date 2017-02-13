#pragma once
#include <boost/test/included/unit_test.hpp>

#include "optimalTextMapping.h"

BOOST_AUTO_TEST_SUITE(OptimalTextMappingTests)

BOOST_AUTO_TEST_CASE(SimpleExample)
{
	using namespace WikiTalkNet::Tests;
	std::vector<WikiTalkNet::Comment> comments1;
	std::vector<WikiTalkNet::Comment> commentsParsed;

	WikiTalkNet::Comment c1;
	c1.User = "Ben";
	c1.Date = "1991-04-09";
	c1.Text = "Dies ist ein Test.";
	comments1.push_back(c1);

	WikiTalkNet::Comment c5;
	c5.User = "Ben";
	c5.Date = "1991-04-09";
	c5.Text = "HalloWelt.";
	comments1.push_back(c5);

	WikiTalkNet::Comment c4;
	c4.User = "Ben";
	c4.Date = "1991-04-09";
	c4.Text = "HalloWelt";
	comments1.push_back(c4);

	WikiTalkNet::Comment c2;
	c2.User = "Ben";
	c2.Date = "1991-04-09";
	c2.Text = "Dies ist ein Test.";

	WikiTalkNet::Comment c3;
	c3.User = "Ben";
	c3.Date = "1991-04-09";
	c3.Text = "HalloWelt";

	commentsParsed.push_back(c3);
	commentsParsed.push_back(c2);

	auto res = computeOptimalTextMapping(comments1, commentsParsed);
	BOOST_CHECK(std::get<1>(res).size() == 1);
	auto& mapping = std::get<0>(res);
	BOOST_CHECK(mapping[0] == 1);
	BOOST_CHECK(mapping[1] == 0);
}

BOOST_AUTO_TEST_SUITE_END()
