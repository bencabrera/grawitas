#pragma once

#include "listCsvOutput.h"

#include <algorithm>

namespace WikiTalkNet {
	void listToHumanReadable(std::ostream& ostr, const ParsedTalkPage& parsedTalkPage);
}
