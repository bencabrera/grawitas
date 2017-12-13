#pragma once

#include <string>
#include <ctime>
#include <list>

namespace Grawitas {
	struct Comment {
		inline Comment()
			:Id(0),ParentId(0)
		{}


		std::string User;
		std::string Text;
		std::tm Date;
		std::string DateString;

		std::size_t Id;
		std::size_t ParentId;

		std::size_t IndentLevel;

		std::string Article;
	};

	using ParsedTalkPage = std::list<std::pair<std::string, std::list<Comment>>>;
}
