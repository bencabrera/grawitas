#pragma once

#include <string>

struct TalkPageResult {
	bool missing;
	std::string title;
	std::string content;
};

struct TalkPageTitle {
	std::string title;
	bool is_archive;
	int i_archive;
};
