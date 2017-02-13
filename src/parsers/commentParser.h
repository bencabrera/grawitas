#pragma once

#include <string>
#include <vector>

#include "../models/comment.h"

namespace WikiTalkNet {

	const std::vector<std::string> STRONG_USER_REG_EXPS = {
		"\\[\\[User:([^\\|\\]]+)[^\\]]*\\]\\] *\\(\\[\\[User talk:.+\\]\\]\\)",    //[[User:Ianmckeachie|Ianmckeachie]] ([[User talk:Ianmckeachie|talk]])
		"\\[\\[User:([^\\|\\]]+)[^\\]]*\\]\\] *\\[\\[User talk:.+\\]\\]",    //[[User:Ianmckeachie|Ianmckeachie]] [[User talk:Ianmckeachie|talk]]
		"\\[\\[Special:Contributions/([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}).*\\]\\] \\(\\[\\[User talk:.+\\]\\]\\)",     //[[Special:Contributions/128.227.41.144|128.227.41.144]] ([[User talk:128.227.41.144|talk]])
	};

	const std::vector<std::string> WEAK_USER_REG_EXPS = {
		"\\[\\[User:([\\w\\s\\.]+).*\\]\\]",               //[[User:Kizzle|kizzle]]
		"\\(\\[\\[User talk:([\\w\\s\\.]+).*\\]\\]\\)",    //([[User talk:Ianmckeachie|talk]])
		"\\[\\[User talk:([\\w\\s\\.]+).*\\]\\]"           //[[User talk:Ianmckeachie|talk]]
	};

	const std::vector<std::string> DATE_REG_EXPS = {
		"([0-9]{2}:[0-9]{2}, [0-9]{2} \\w{3} [0-9]{4} \\(UTC\\))",          //04:29, 22 Oct 2004 (UTC)
		"([0-9]{2}:[0-9]{2}, \\w{3} [0-9]{1,2}, [0-9]{4} \\(UTC\\))",       //06:06, Nov 1, 2004 (UTC)
		"([0-9]{2}:[0-9]{2}, [0-9]{4} \\w{3} [0-9]{1,2} \\(UTC\\))",        //15:33, 2004 Nov 13 (UTC)
		"([0-9]{2}:[0-9]{2}, [0-9]{1,2} \\w{1,12} [0-9]{4} \\(UTC\\))",     //11:57, 9 October 2015 (UTC)
		"([0-9]{2}:[0-9]{2}, [0-9]{1,2} \\w{1,12} [0-9]{4} \\(UTC\\))"     //00:00, 16 February 2011 (UTC)
	};

	const std::string LINE_END_REG_EXP = "*[\\.;]*$"; 
	const std::vector<std::string> FILLER_REG_EXP = {
		"[\\s]{0,5}[,;\\|]?[\\s]{0,5}",
	}; 

	Comment parseCommentFromLine(const std::string& line);

	bool parseUserAndDateAtEnd(const std::string& line, Comment& comment);
	bool parseOnlyUserAtEnd(const std::string& line, Comment& comment);
	bool parseOnlyDateAtEnd(const std::string& line, Comment& comment);
}
