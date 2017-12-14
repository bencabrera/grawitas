#pragma once

#include "listCsvOutput.h"

#include <algorithm>

namespace Grawitas {
	void listToHumanReadable(std::ostream& ostr, const std::vector<Comment>& parsedTalkPage);
}
