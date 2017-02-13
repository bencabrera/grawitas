#include "commentParser.h"

#include <regex>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "regexCombinatoricalParser.h"

namespace WikiTalkNet {
	Comment parseCommentFromLine(const std::string& line) {
		Comment comment;
		comment.Text = line;

		if(parseUserAndDateAtEnd(line, comment))
			return comment;
		if(parseOnlyUserAtEnd(line, comment))
			return comment;
		if(parseOnlyDateAtEnd(line, comment))
			return comment;

		boost::trim_if(comment.User, boost::is_any_of(" \t"));

		return comment;
	}


	bool parseUserAndDateAtEnd(const std::string &line, Comment &comment) 
	{
		std::vector<std::string> USER_REG_EXPS;
		USER_REG_EXPS.insert(USER_REG_EXPS.end(), STRONG_USER_REG_EXPS.begin(), STRONG_USER_REG_EXPS.end());
		USER_REG_EXPS.insert(USER_REG_EXPS.end(), WEAK_USER_REG_EXPS.begin(), WEAK_USER_REG_EXPS.end());

		RegexCombinatoricalParser combiParser({ USER_REG_EXPS, FILLER_REG_EXP, DATE_REG_EXPS, { LINE_END_REG_EXP } });

		auto result = combiParser(line);
		if(result.Matched)
		{
			comment.User = result.Matches[1];
			comment.DateString = result.Matches[2];
			return true;
		}
		else
			return false;
	}

	bool parseOnlyUserAtEnd(const std::string &line, Comment &comment) 
	{
		std::vector<std::string> USER_REG_EXPS;
		USER_REG_EXPS.insert(USER_REG_EXPS.end(), STRONG_USER_REG_EXPS.begin(), STRONG_USER_REG_EXPS.end());
		USER_REG_EXPS.insert(USER_REG_EXPS.end(), WEAK_USER_REG_EXPS.begin(), WEAK_USER_REG_EXPS.end());

		RegexCombinatoricalParser combiParser({ USER_REG_EXPS, { LINE_END_REG_EXP } });

		auto result = combiParser(line);
		if(result.Matched)
		{
			comment.User = result.Matches[1];
			return true;
		}
		else
			return false;
	}

	bool parseOnlyDateAtEnd(const std::string &line, Comment &comment) 
	{
		RegexCombinatoricalParser combiParser({ DATE_REG_EXPS, { LINE_END_REG_EXP } });

		auto result = combiParser(line);
		if(result.Matched)
		{
			comment.User = result.Matches[1];
			return true;
		}
		else
			return false;	
	}
}
