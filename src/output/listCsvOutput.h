#pragma once

#include <iostream>

#include "../talkPageParser/models.h"

namespace Grawitas {
	void listToCsv(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage);
}

