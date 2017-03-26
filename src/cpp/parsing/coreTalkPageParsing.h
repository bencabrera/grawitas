#pragma once 

#include "../models/parsedTalkPage.h"
#include "../models/comment.h"

namespace Grawitas {
	ParsedTalkPage parse_talk_page(std::istream& ostr);
	ParsedTalkPage parse_talk_page(const std::string& content);
	void calculate_ids(std::list<Comment>& comments, std::size_t& startId);
}
