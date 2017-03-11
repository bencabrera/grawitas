#pragma once

#include <string>
#include <vector>

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

	const std::vector<std::string> FormatHumanReadableStrings = {
		"Comment List (JSON)",
		"Comment List (Human readable)",
		"Comment List (Csv)",
		"User Network (GML)",
		"User Network (GraphML)",
		"User Network (GraphViz)",
		"Comment Network (GML)",
		"Comment Network (GraphML)",
		"Comment Network (GraphViz)",
		"Two Mode Network (GML)",
		"Two Mode Network (GraphML)",
		"Two Mode Network (GraphViz)"
	};

	const std::vector<std::string> FormatParameterStrings = {
		"comment-list-json",
		"comment-list-human-readable",
		"comment-list-csv",
		"user-network-gml",
		"user-network-graphml",
		"user-network-graphviz",
		"comment-network-gml",
		"comment-network-graphml",
		"comment-network-graphviz",
		"two-mode-network-gml",
		"two-mode-network-graphml",
		"two-mode-network-graphviz"
	};

	std::string format_to_readable(Format format);
	Format readable_to_format(std::string format_str);
	Format parameter_to_format(std::string parameter_str);
}
