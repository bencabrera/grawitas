#pragma once

#include <iostream>

#include "../models/parsedTalkPage.h"

namespace WikiTalkNet {
	void listToCsv(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage);
}

