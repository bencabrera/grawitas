#include "graphGraphmlOutput.h"

#include <boost/graph/graphml.hpp>

#include "dateOutput.h"

namespace Grawitas {

	void graphToGraphml(std::ostream& ostr, UserGraph& g)
	{
		auto vLabelMap = boost::get(boost::vertex_name, g);
		auto eLabelMap = boost::get(boost::edge_weight, g);

		boost::dynamic_properties dp;
		dp.property("userName", vLabelMap);
		dp.property("weight", eLabelMap);

		boost::write_graphml(ostr, g, dp);
	}

	void graphToGraphml(std::ostream& ostr, CommentGraph& g)
	{
		auto vLabelMap = boost::get(boost::vertex_name, g);

		std::map<CommentGraph::vertex_descriptor, std::string> username_map, date_map, text_map;
		for(std::size_t i = 0; i < boost::num_vertices(g); i++)
		{
			username_map.insert({ i, get(vLabelMap, i).User });
			date_map.insert({ i, to_iso_8601(get(vLabelMap, i).Date) });
			text_map.insert({ i, get(vLabelMap, i).Text });
		}

		auto username_prop_map = boost::associative_property_map<std::map<CommentGraph::vertex_descriptor, std::string>> (username_map); 
		auto date_prop_map = boost::associative_property_map<std::map<CommentGraph::vertex_descriptor, std::string>> (date_map); 
		auto text_prop_map = boost::associative_property_map<std::map<CommentGraph::vertex_descriptor, std::string>> (text_map); 

		boost::dynamic_properties dp;
		dp.property("user", username_prop_map);
		dp.property("date", date_prop_map);
		dp.property("text", text_prop_map);
		
		boost::write_graphml(ostr, g, dp);
	}

	void graphToGraphml(std::ostream& ostr, TwoModeGraph& g)
	{
		auto vLabelMap = boost::get(boost::vertex_name, g);

		std::map<CommentGraph::vertex_descriptor, std::string> username_map, text_map;
		std::map<CommentGraph::vertex_descriptor, bool> is_user_map;

		for(std::size_t i = 0; i < boost::num_vertices(g); i++)
		{
			is_user_map.insert({ i, get(vLabelMap, i).IsUser });
			if(get(vLabelMap, i).IsUser)
				username_map.insert({ i, get(vLabelMap, i).Username });
			else
				text_map.insert({ i, get(vLabelMap, i).Com.Text });
		}

		auto is_user_prop_map = boost::associative_property_map<std::map<CommentGraph::vertex_descriptor, bool>> (is_user_map); 
		auto username_prop_map = boost::associative_property_map<std::map<CommentGraph::vertex_descriptor, std::string>> (username_map); 
		auto text_prop_map = boost::associative_property_map<std::map<CommentGraph::vertex_descriptor, std::string>> (text_map); 

		boost::dynamic_properties dp;
		dp.property("is_user", is_user_prop_map);
		dp.property("user", username_prop_map);
		dp.property("text", text_prop_map);
		
		boost::write_graphml(ostr, g, dp);
	}
}
