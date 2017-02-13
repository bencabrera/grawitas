#pragma once

#include <boost/program_options.hpp>
#include "../models/parsedTalkPage.h"

namespace Grawitas {

	void outputWrapper(const boost::program_options::variables_map& vm, const ParsedTalkPage& parsedTalkPage);

}
