#pragma once

#include <iostream>

#include "../models/parsedTalkPage.h"

namespace Grawitas {
	void listToCsv(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage);
}

