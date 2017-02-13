#pragma once

#include <iostream>

#include "../models/graph.h"

namespace Grawitas {

	void graphToGml(std::ostream& ostr, const UserGraph& g);
	void graphToGml(std::ostream& ostr, const CommentGraph& g);
	void graphToGml(std::ostream& ostr, const TwoModeGraph& g);

}
