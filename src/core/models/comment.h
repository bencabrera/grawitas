#pragma once

#include <string>
#include <ctime>

namespace Grawitas {

	class Comment {
		public:
			Comment();

			std::string User;
			std::string Text;
			std::tm Date;
			std::string DateString;

			std::size_t Id;
			std::size_t ParentId;

			std::size_t IndentLevel;

			std::string Summary() const;
	};
}
