#pragma once

#include <vector>
#include <string>

struct TalkPageResult {
	bool missing;
	std::string title;
	std::string content;
};

std::vector<TalkPageResult> get_pages_from_wikipedia(std::vector<std::string> page_titles);
