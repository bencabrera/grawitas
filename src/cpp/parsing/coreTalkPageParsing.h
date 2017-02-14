#pragma once 

#include "../models/parsedTalkPage.h"
#include "../models/comment.h"

namespace Grawitas {
	ParsedTalkPage parseTalkPage(std::istream& ostr);
	ParsedTalkPage parseTalkPage(const std::string& content);
	void calculateIds(std::list<Comment>& comments, std::size_t& startId);
}
