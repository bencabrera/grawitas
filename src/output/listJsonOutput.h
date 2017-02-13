#pragma once

#include <iostream>

#include "../models/parsedTalkPage.h"

namespace WikiTalkNet {
	void listToJson(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage);
}

