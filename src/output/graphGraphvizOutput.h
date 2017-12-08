#pragma once

#include <iostream>

#include "../networks/graph.h"

namespace Grawitas {

	void graphToGraphviz(std::ostream& ostr, const UserGraph& g);
	void graphToGraphviz(std::ostream& ostr, const CommentGraph& g);
	void graphToGraphviz(std::ostream& ostr, const TwoModeGraph& g);

}
