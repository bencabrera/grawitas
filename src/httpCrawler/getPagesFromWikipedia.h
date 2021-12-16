#pragma once

#include <vector>
#include <string>

struct TalkPageResult {
	bool missing;
    bool invalid; 
	bool is_archive;
	int i_archive;

	std::string full_title;
	std::string title;
	std::string content;
};

std::vector<TalkPageResult> get_pages_from_wikipedia(std::vector<std::string> page_titles);
