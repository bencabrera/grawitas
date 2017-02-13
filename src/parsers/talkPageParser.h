#pragma once 

#include "../models/parsedTalkPage.h"
#include "../models/comment.h"

namespace WikiTalkNet {
	ParsedTalkPage parseTalkPage(std::istream& ostr);
	void calculateIds(std::list<Comment>& comments, std::size_t& startId);
}
