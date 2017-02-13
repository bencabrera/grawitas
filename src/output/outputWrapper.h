#pragma once

#include <boost/program_options.hpp>
#include "../models/parsedTalkPage.h"

namespace WikiTalkNet {

	void outputWrapper(const boost::program_options::variables_map& vm, const ParsedTalkPage& parsedTalkPage);

}
