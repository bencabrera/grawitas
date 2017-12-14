#pragma once 

#include "../talkPageParser/models.h"
#include <vector>

namespace Grawitas {
	std::vector<Comment> parse_talk_page(std::istream& ostr);
	std::vector<Comment> parse_talk_page(const std::string& content);
	void calculate_ids(std::vector<Comment>& comments, std::size_t& startId);
	std::vector<std::tuple<std::string, std::string, int>> split_into_sections(const std::string& content);
}
