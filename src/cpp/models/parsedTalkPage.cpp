#include "parsedTalkPage.h"

namespace Grawitas {

	std::list<Comment> flattenParsedTalkPage(const ParsedTalkPage& parsedTalkPage)
	{
		std::list<Comment> rtn;
		std::size_t counter = 0;
		for(auto& paragraph : parsedTalkPage)
		{
			for(auto& comment : paragraph.second)
			{
				rtn.push_back(comment);
				rtn.back().Id += counter;
				if(rtn.back().ParentId != 0)
					rtn.back().ParentId += counter;
			}
			counter += paragraph.second.size();
		}

		return rtn;
	}

}
