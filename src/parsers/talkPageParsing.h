#pragma once

#include <iostream>
#include <list>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <string>
#include <iomanip>
#include <regex>
#include <stack>

#include "../models/comment.h"

namespace WikiTalkNet {

	using HeaderDescriptor =  std::vector<std::string>;
	using ParagraphWithHeader = std::pair<HeaderDescriptor, std::string>;

	// steps of parsing talk page
	void splitByHeader(std::list<ParagraphWithHeader>& input, unsigned numberOfEqualitySigns);
	std::list<std::string> splitIntoComments(const std::string& str);
	std::list<Comment> parseLinesIntoComments(const std::list<std::string>& lines);

	// helper functions
	void splitEachByRegex(std::list<std::string>& strs, const std::regex splitRegexStd, bool cutInFront);
	void trimEach(std::list<std::string>& strs, std::string charsToRemove);
	void removeEmpty(std::list<std::string>& strs);
	void flattenNewlines(std::list<std::string>& strs);
}
