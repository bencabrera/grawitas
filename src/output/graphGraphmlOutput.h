#pragma once

#include <iostream>

#include "../networks/graph.h"

namespace Grawitas {

	void graphToGraphml(std::ostream& ostr, UserGraph& g);
	void graphToGraphml(std::ostream& ostr, CommentGraph& g);
	void graphToGraphml(std::ostream& ostr, TwoModeGraph& g);

}
