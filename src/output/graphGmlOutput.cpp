#include "graphGmlOutput.h"

namespace WikiTalkNet {

	void graphToGml(std::ostream& ostr, const UserGraph& g)
	{
		auto vertex_name_map = boost::get(boost::vertex_name, g);
		auto edge_weight_map = boost::get(boost::edge_weight, g);

		ostr << "graph [" << std::endl;
		ostr << "\t directed 1" << std::endl;
		boost::graph_traits<UserGraph>::vertex_iterator vIt, vItEnd;
		unsigned vIdx = 0;

		for(boost::tie(vIt, vItEnd) = vertices(g); vIt != vItEnd; ++vIt)
		{
			ostr << "\t node [" << std::endl;
			ostr << "\t\t id " << vIdx << std::endl;
			ostr << "\t\t label \"" << boost::get(vertex_name_map, *vIt) << "\"" << std::endl;
			ostr << "\t ]" << std::endl;
			vIdx++;
		}
		for(boost::tie(vIt, vItEnd) = vertices(g); vIt != vItEnd; ++vIt)
		{
			boost::graph_traits<UserGraph>::out_edge_iterator eIt, eItEnd;
			for(boost::tie(eIt, eItEnd) = boost::out_edges(*vIt, g); eIt != eItEnd; ++eIt)
			{
				ostr << "\t edge [" << std::endl;
				ostr << "\t\t source " << boost::source(*eIt, g) << std::endl;
				ostr << "\t\t target " << boost::target(*eIt, g) << std::endl;
				ostr << "\t\t weight \"" << boost::get(edge_weight_map,*eIt) << "\"" << std::endl;
				ostr << "\t ]" << std::endl;
			}
		}
		ostr << "]";
	}


	void graphToGml(std::ostream& ostr, const CommentGraph& g)
	{
		auto vertex_name_map = boost::get(boost::vertex_name, g);

		ostr << "graph [" << std::endl;
		ostr << "\t directed 1" << std::endl;
		boost::graph_traits<CommentGraph>::vertex_iterator vIt, vItEnd;
		unsigned vIdx = 0;

		for(boost::tie(vIt, vItEnd) = vertices(g); vIt != vItEnd; ++vIt)
		{
			ostr << "\t node [" << std::endl;
			ostr << "\t\t id " << vIdx << std::endl;
			ostr << "\t\t label \"" << boost::get(vertex_name_map, *vIt).Summary() << "\"" << std::endl;
			ostr << "\t ]" << std::endl;
			vIdx++;
		}
		for(boost::tie(vIt, vItEnd) = vertices(g); vIt != vItEnd; ++vIt)
		{
			boost::graph_traits<CommentGraph>::out_edge_iterator eIt, eItEnd;
			for(boost::tie(eIt, eItEnd) = boost::out_edges(*vIt, g); eIt != eItEnd; ++eIt)
			{
				ostr << "\t edge [" << std::endl;
				ostr << "\t\t source " << boost::source(*eIt, g) << std::endl;
				ostr << "\t\t target " << boost::target(*eIt, g) << std::endl;
				ostr << "\t ]" << std::endl;
			}
		}
		ostr << "]";
	}

	void graphToGml(std::ostream& ostr, const TwoModeGraph& g)
	{
		auto vertex_name_map = boost::get(boost::vertex_name, g);

		ostr << "graph [" << std::endl;
		ostr << "\t directed 1" << std::endl;
		boost::graph_traits<CommentGraph>::vertex_iterator vIt, vItEnd;
		unsigned vIdx = 0;

		for(boost::tie(vIt, vItEnd) = vertices(g); vIt != vItEnd; ++vIt)
		{
			auto& obj = boost::get(vertex_name_map, *vIt);
			if(obj.IsUser)
			{
				ostr << "\t node [" << std::endl;
				ostr << "\t\t id " << vIdx << std::endl;
				ostr << "\t\t type \"user\"" << std::endl;
				ostr << "\t\t label \"" << obj.Username << "\"" << std::endl;
				ostr << "\t ]" << std::endl;
			} else {
				ostr << "\t node [" << std::endl;
				ostr << "\t\t id " << vIdx << std::endl;
				ostr << "\t\t type \"comment\"" << std::endl;
				ostr << "\t\t label \"" << obj.Com.Summary() << "\"" << std::endl;
				ostr << "\t ]" << std::endl;
			}
				
			vIdx++;
		}

		for(boost::tie(vIt, vItEnd) = vertices(g); vIt != vItEnd; ++vIt)
		{
			boost::graph_traits<CommentGraph>::out_edge_iterator eIt, eItEnd;
			for(boost::tie(eIt, eItEnd) = boost::out_edges(*vIt, g); eIt != eItEnd; ++eIt)
			{
				ostr << "\t edge [" << std::endl;
				ostr << "\t\t source " << boost::source(*eIt, g) << std::endl;
				ostr << "\t\t target " << boost::target(*eIt, g) << std::endl;
				ostr << "\t ]" << std::endl;
			}
		}
		ostr << "]";
	}

}
