#include "formats.h"

#include <iomanip>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace Grawitas {

	const std::map<Format, std::string> FormatFileExtensions = {
		{ USER_NETWORK_GML, ".user_network.gml" },
		{ USER_NETWORK_GRAPHML, ".user_network.graphml" },
		{ USER_NETWORK_GRAPHVIZ, ".user_network.dot" },
		{ COMMENT_NETWORK_GML, ".comment_network.gml" },
		{ COMMENT_NETWORK_GRAPHML, ".comment_network.graphml" },
		{ COMMENT_NETWORK_GRAPHVIZ, ".comment_network.dot" },
		{ COMMENT_LIST_CSV, ".comment_list.csv" },
		{ COMMENT_LIST_HUMAN_READABLE, ".comment_list.txt" },
		{ COMMENT_LIST_JSON, ".comment_list.json" },
		{ TWO_MODE_NETWORK_GML, ".two_mode_network.gml" },
		{ TWO_MODE_NETWORK_GRAPHML, ".two_mode_network.graphml" },
		{ TWO_MODE_NETWORK_GRAPHVIZ, ".two_mode_network.dot" },
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

	std::string format_to_readable(Format format)
	{
		if(format == COMMENT_LIST_JSON)
			return "Comment List (JSON)";
		if(format == COMMENT_LIST_HUMAN_READABLE)
			return "Comment List (Human readable)";
		if(format == COMMENT_LIST_CSV)
			return "Comment List (Csv)";
		if(format == USER_NETWORK_GML)
			return "User Network (GML)";
		if(format == USER_NETWORK_GRAPHML)
			return "User Network (GraphML)";
		if(format == USER_NETWORK_GRAPHVIZ)
			return "User Network (GraphViz)";
		if(format == COMMENT_NETWORK_GML)
			return "Comment Network (GML)";
		if(format == COMMENT_NETWORK_GRAPHML)
			return "Comment Network (GraphML)";
		if(format == COMMENT_NETWORK_GRAPHVIZ)
			return "Comment Network (GraphViz)";
		if(format == TWO_MODE_NETWORK_GML)
			return "Two Mode Network (GML)";
		if(format == TWO_MODE_NETWORK_GRAPHML)
			return "Two Mode Network (GraphML)";
		//if(format == TWO_MODE_NETWORK_GRAPHVIZ)
		return "Two Mode Network (GraphViz)";
	}

	Format readable_to_format(std::string format_str)
	{
		if(format_str == "Comment List (JSON)")
			return COMMENT_LIST_JSON;
		if(format_str == "Comment List (Human readable)")
			return COMMENT_LIST_HUMAN_READABLE;
		if(format_str == "Comment List (Csv)")
			return COMMENT_LIST_CSV;
		if(format_str == "User Network (GML)")
			return USER_NETWORK_GML;
		if(format_str == "User Network (GraphML)")
			return USER_NETWORK_GRAPHML;
		if(format_str == "User Network (GraphViz)")
			return USER_NETWORK_GRAPHVIZ;
		if(format_str == "Comment Network (GML)")
			return COMMENT_NETWORK_GML;
		if(format_str == "Comment Network (GraphML)")
			return COMMENT_NETWORK_GRAPHML;
		if(format_str == "Comment Network (GraphViz)")
			return COMMENT_NETWORK_GRAPHVIZ;
		if(format_str == "Two Mode Network (GML)")
			return TWO_MODE_NETWORK_GML;
		if(format_str == "Two Mode Network (GraphML)")
			return TWO_MODE_NETWORK_GRAPHML;

		return TWO_MODE_NETWORK_GRAPHVIZ;
	}

	Format parameter_to_format(std::string parameter_str)
	{
		if(parameter_str == "comment-list-json")
			return COMMENT_LIST_JSON;
		if(parameter_str == "comment-list-human-readable")
			return COMMENT_LIST_HUMAN_READABLE;
		if(parameter_str == "comment-list-csv")
			return COMMENT_LIST_CSV;
		if(parameter_str == "user-network-gml")
			return USER_NETWORK_GML;
		if(parameter_str == "user-network-graphml")
			return USER_NETWORK_GRAPHML;
		if(parameter_str == "user-network-graphviz")
			return USER_NETWORK_GRAPHVIZ;
		if(parameter_str == "comment-network-gml")
			return COMMENT_NETWORK_GML;
		if(parameter_str == "comment-network-graphml")
			return COMMENT_NETWORK_GRAPHML;
		if(parameter_str == "comment-network-graphviz")
			return COMMENT_NETWORK_GRAPHVIZ;
		if(parameter_str == "two-mode-network-gml")
			return TWO_MODE_NETWORK_GML;
		if(parameter_str == "two-mode-network-graphml")
			return TWO_MODE_NETWORK_GRAPHML;

		return TWO_MODE_NETWORK_GRAPHVIZ;
	}

	std::string safeEncodeTitleToFilename(const std::string title)
	{
		std::string str;
		if(title.substr(0, 5) == "Talk:")
			str = title.substr(5, title.length()-5);
		else
			str = title;

		boost::trim(str);
		boost::replace_all(str, " ", "_");

		std::ostringstream escaped;
		escaped.fill('0');
		escaped << std::hex;

		for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i) {
			std::string::value_type c = (*i);

			// Keep alphanumeric and other accepted characters intact
			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
				escaped << c;
				continue;
			}

			// Any other characters are percent-encoded
			escaped << std::uppercase;
			escaped << '%' << std::setw(2) << int((unsigned char) c);
			escaped << std::nouppercase;
		}

		return escaped.str();
	}
}
