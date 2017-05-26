#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <string>
#include "comment.h"
#include <boost/graph/graphml.hpp>


namespace Grawitas {
	struct UserOrCommentNode {
		bool IsUser;

		std::string Username;
		Comment Com;
	};


	typedef boost::property< boost::vertex_name_t, std::string > UserVertexProperties;
	typedef boost::property< boost::edge_weight_t, double > UserEdgeProperties;
	typedef boost::adjacency_list<boost::setS, boost::vecS, boost::directedS, UserVertexProperties, UserEdgeProperties> UserGraph;

	typedef boost::property< boost::vertex_name_t, Comment> CommentVertexProperties;
	typedef boost::adjacency_list<boost::setS, boost::vecS, boost::directedS, CommentVertexProperties, boost::no_property> CommentGraph;

	typedef boost::property< boost::vertex_name_t, UserOrCommentNode> TwoModeVertexProperties;
	typedef boost::adjacency_list<boost::setS, boost::vecS, boost::directedS, TwoModeVertexProperties, boost::no_property> TwoModeGraph;
}

