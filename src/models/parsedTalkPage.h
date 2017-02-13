#include <list>
#include <vector>
#include <string>

#include "comment.h"

namespace WikiTalkNet {

	// todo: explanation
	using ParsedTalkPage = std::list<std::pair<std::vector<std::string>, std::list<Comment>>>;

	std::list<Comment> flattenParsedTalkPage(const ParsedTalkPage& parsedTalkPage);

}
