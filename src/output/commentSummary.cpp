#include "commentSummary.h"
#include <algorithm>
#include <sstream>
#include "dateOutput.h"

namespace Grawitas {
	std::string comment_summary(const Comment& comment)
	{
		auto text = comment.Text;
		std::replace(text.begin(), text.end(), '"', '\'');
		std::stringstream ss;
		ss << to_human_readable(comment.Date);
		return "User:" + comment.User + "; Date:" + ss.str() + "; Text:" + text;
	}
}
