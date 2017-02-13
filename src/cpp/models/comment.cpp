#include "comment.h"

#include <algorithm>
#include <sstream>
#include "../output/dateOutput.h"

namespace Grawitas {

	Comment::Comment()
		:Id(0),ParentId(0)
	{}

	std::string Comment::Summary() const
	{
		auto text = Text;
		std::replace(text.begin(), text.end(), '"', '\'');
		std::stringstream ss;
		ss << to_human_readable(Date);
		return "User:" + User + "; Date:" + ss.str() + "; Text:" + text;
	}
}
