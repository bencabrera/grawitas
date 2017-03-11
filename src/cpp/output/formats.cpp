#include "formats.h"

namespace Grawitas {
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
}
