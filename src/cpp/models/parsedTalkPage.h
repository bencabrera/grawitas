#include <list>
#include <vector>
#include <string>

#include "comment.h"

namespace Grawitas {

	// todo: explanation
	using ParsedTalkPage = std::list<std::pair<std::string, std::list<Comment>>>;

	std::list<Comment> flattenParsedTalkPage(const ParsedTalkPage& parsedTalkPage);

}
