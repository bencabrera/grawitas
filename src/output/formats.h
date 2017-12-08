#pragma once

#include <string>
#include <vector>
#include <map>

namespace Grawitas {
	enum Format {
		USER_NETWORK_GML,
		USER_NETWORK_GRAPHML,
		USER_NETWORK_GRAPHVIZ,
		COMMENT_NETWORK_GML,
		COMMENT_NETWORK_GRAPHML,
		COMMENT_NETWORK_GRAPHVIZ,
		TWO_MODE_NETWORK_GML,
		TWO_MODE_NETWORK_GRAPHML,
		TWO_MODE_NETWORK_GRAPHVIZ,
		COMMENT_LIST_JSON,
		COMMENT_LIST_HUMAN_READABLE,
		COMMENT_LIST_CSV
	};

	extern const std::map<Format, std::string> FormatFileExtensions;
	extern const std::vector<std::string> FormatHumanReadableStrings;
	extern const std::vector<std::string> FormatParameterStrings;

	std::string format_to_readable(Format format);
	Format readable_to_format(std::string format_str);
	Format parameter_to_format(std::string parameter_str);

	std::string safeEncodeTitleToFilename(const std::string title);
}
