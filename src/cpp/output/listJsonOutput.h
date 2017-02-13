#pragma once

#include <iostream>

#include "../models/parsedTalkPage.h"

namespace Grawitas {
	void listToJson(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage);
}

