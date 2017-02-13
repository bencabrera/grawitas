#include "graphGraphmlOutput.h"

#include <boost/graph/graphviz.hpp>

namespace WikiTalkNet {

	void graphToGraphviz(std::ostream& ostr, const UserGraph& g)
	{
		auto vertex_name_map = boost::get(boost::vertex_name, g);
		auto edge_weight_map = boost::get(boost::edge_weight, g);

		auto vertex_label_writer = boost::make_label_writer(vertex_name_map);
		auto edge_label_writer = boost::make_label_writer(edge_weight_map);

		boost::write_graphviz (ostr, g, vertex_label_writer, edge_label_writer);
	}

	void graphToGraphviz(std::ostream& ostr, const CommentGraph& g)
	{
		using VertexDescriptor = CommentGraph::vertex_descriptor;
		auto vertex_name_map = boost::get(boost::vertex_name, g);

		auto vertex_label_writer = [&vertex_name_map](std::ostream& out, const VertexDescriptor& v) 
		{
			out << "[label=\"" << vertex_name_map[v].Summary() << "\"]";
		};

		boost::write_graphviz (ostr, g, vertex_label_writer);
	}

	void graphToGraphviz(std::ostream& ostr, const TwoModeGraph& g)
	{
		using VertexDescriptor = CommentGraph::vertex_descriptor;
		auto vertex_name_map = boost::get(boost::vertex_name, g);

		auto vertex_label_writer = [&vertex_name_map](std::ostream& out, const VertexDescriptor& v) 
		{
			auto& obj = vertex_name_map[v];
			if(obj.IsUser)
				out << "[label=\"User:" << obj.Username << "\", shape=box]";
			else
				out << "[label=\"" << obj.Com.Summary() << "\"]";
		};

		boost::write_graphviz (ostr, g, vertex_label_writer);
	}

}
