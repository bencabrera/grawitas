#include "graphComputationCache.h"

namespace Grawitas {
	GraphComputationCache::GraphComputationCache(const ParsedTalkPage& parsedTalkPage)
		:_parsedTalkPage(parsedTalkPage),
		_hasUserGraph(false),
		_hasCommentGraph(false),
		_userGraph(NULL),
		_commentGraph(NULL),
		_twoModeGraph(NULL)
	{}

	GraphComputationCache::~GraphComputationCache()
	{
		if(_hasUserGraph && _userGraph != NULL)
			delete _userGraph;
		if(_hasCommentGraph && _commentGraph != NULL)
			delete _commentGraph;
		if(_hasTwoModeGraph && _twoModeGraph != NULL)
			delete _twoModeGraph;
	}

	UserGraph& GraphComputationCache::GetUserGraph()
	{
		if(!_hasUserGraph)
		{
			computeUserGraph();
			_hasUserGraph = true;
		}

		return *_userGraph;
	}

	CommentGraph& GraphComputationCache::GetCommentGraph() 
	{
		if(!_hasCommentGraph)
		{
			computeCommentGraph();
			_hasCommentGraph = true;
		}

		return *_commentGraph;
	}

	TwoModeGraph& GraphComputationCache::GetTwoModeGraph() 
	{
		if(!_hasTwoModeGraph)
		{
			computeTwoModeGraph();
			_hasTwoModeGraph = true;
		}

		return *_twoModeGraph;
	}

	void GraphComputationCache::computeUserGraph()
	{
		using VertexDescriptor = UserGraph::vertex_descriptor;

		_userGraph = new UserGraph();
		auto& g = *_userGraph;
		std::map<std::string, VertexDescriptor> nameToVertexMapper;

		for(auto& paragraph : _parsedTalkPage)
		{
			auto allComments = paragraph.second;
			std::map<std::size_t, Comment> idToCommentMap;
			for(auto c : allComments)
			{
				idToCommentMap.insert({ c.Id, c });
			}

			auto edge_weight_map = boost::get(boost::edge_weight, g);
			auto vertex_name_map = boost::get(boost::vertex_name, g);
			for(auto& curComment : allComments)
			{
				VertexDescriptor vFrom = 0;
				VertexDescriptor vTo = 0;

				auto nameFrom = curComment.User;
				if(nameFrom != "")
				{
					auto mapIt = nameToVertexMapper.find(nameFrom);
					if(mapIt == nameToVertexMapper.end())
					{
						auto v = boost::add_vertex(g);
						nameToVertexMapper.insert({ nameFrom, v });
						vFrom = v;
						boost::put(vertex_name_map, v, nameFrom);
					}
					else
						vFrom = mapIt->second;
				}


				// TODO: can the be further optimized

				if(curComment.ParentId != 0)
				{
					auto nameTo = idToCommentMap.at(curComment.ParentId).User;
					if(nameTo != "")
					{
						auto mapIt = nameToVertexMapper.find(nameTo);
						if(mapIt == nameToVertexMapper.end())
						{
							auto v = boost::add_vertex(g);
							nameToVertexMapper.insert({ nameTo, v });
							vTo = v;
							boost::put(vertex_name_map, v, nameTo);
						}
						else
							vTo = mapIt->second;
					}

					if(nameFrom != "" && nameTo != "")
					{
						auto res = boost::add_edge(vFrom, vTo, g);
						if(res.second)
							boost::put(edge_weight_map, res.first, 1);
						else
							boost::put(edge_weight_map, res.first, boost::get(edge_weight_map, res.first) + 1);
					}
				}
			}
		}
	}	

	void GraphComputationCache::computeCommentGraph()
	{
		using VertexDescriptor = CommentGraph::vertex_descriptor;

		_commentGraph = new CommentGraph();
		auto& g = *_commentGraph;
		std::map<std::size_t, VertexDescriptor> commentIdToVertexMapper;

		for(auto& paragraph : _parsedTalkPage)
		{
			auto allComments = paragraph.second;

			auto vertex_name_map = boost::get(boost::vertex_name, g);

			for(auto& curComment : allComments)
			{
				// add comment as node in the graph
				auto idFrom = curComment.Id;
				VertexDescriptor vFrom = boost::add_vertex(g);
				commentIdToVertexMapper.insert({ idFrom, vFrom });
				boost::put(vertex_name_map, vFrom, curComment);

				// if ParentId != 0 add edge to graph
				if(curComment.ParentId != 0)
				{
					auto idTo = curComment.ParentId;
					VertexDescriptor vTo = commentIdToVertexMapper.find(idTo)->second;

					boost::add_edge(vFrom, vTo, g);
				}

			}
		}
	}

	void GraphComputationCache::computeTwoModeGraph()
	{
		using VertexDescriptor = TwoModeGraph::vertex_descriptor;

		_twoModeGraph = new TwoModeGraph();
		auto& g = *_twoModeGraph;
		std::map<std::string, VertexDescriptor> nameToVertexMapper;
		std::map<std::size_t, VertexDescriptor> commentIdToVertexMapper;
		auto vertex_name_map = boost::get(boost::vertex_name, g);

		for(auto& section : _parsedTalkPage)
		{
			auto allComments = section.second;

			for(auto& curComment : allComments)
			{
				// add comment as node in the graph
				auto idFrom = curComment.Id;
				VertexDescriptor vFrom = boost::add_vertex(g);
				commentIdToVertexMapper.insert({ idFrom, vFrom });
				boost::put(vertex_name_map, vFrom, UserOrCommentNode{ false, "", curComment });

				// if ParentId != 0 add edge to graph
				if(curComment.ParentId != 0)
				{
					auto idTo = curComment.ParentId;
					VertexDescriptor vTo = commentIdToVertexMapper.find(idTo)->second;

					boost::add_edge(vFrom, vTo, g);
				}

				// add connection to user node
				VertexDescriptor user_vertex;
				auto user_it = nameToVertexMapper.find(curComment.User);	
				if(user_it == nameToVertexMapper.end())
				{
					user_vertex = boost::add_vertex(g);
					boost::put(vertex_name_map, user_vertex, UserOrCommentNode{ true, curComment.User });
					nameToVertexMapper.insert({ curComment.User, user_vertex });
				} else {
					user_vertex = user_it->second;
				}

				boost::add_edge(user_vertex, vFrom, g);
			}
		}
	}
}
