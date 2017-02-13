#pragma once

#include <iostream>

#include "../models/graph.h"

namespace WikiTalkNet {

	void graphToGraphml(std::ostream& ostr, UserGraph& g);
	void graphToGraphml(std::ostream& ostr, CommentGraph& g);
	void graphToGraphml(std::ostream& ostr, TwoModeGraph& g);

}
