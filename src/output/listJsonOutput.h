#pragma once

#include <iostream>

#include "../talkPageParser/models.h"

namespace Grawitas {
	void listToJson(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage);
}

