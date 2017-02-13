#include "outputWrapper.h"

#include <string>
#include <iostream>
#include <fstream>

#include "listCsvOutput.h"
#include "graphGmlOutput.h"
#include "graphGraphmlOutput.h"
#include "graphGraphvizOutput.h"
#include "listCsvOutput.h"
#include "listHumanReadableOutput.h"
#include "listJsonOutput.h"

#include "../graphComputation/graphComputationCache.h"

namespace WikiTalkNet {

	void outputWrapper(const boost::program_options::variables_map& vm, const ParsedTalkPage& parsedTalkPage)
	{
		auto cache = GraphComputationCache(parsedTalkPage);


		if(vm.count("user-network-gml"))
		{
			std::ofstream file(vm["user-network-gml"].as<std::string>());
			graphToGml(file, cache.GetUserGraph());
		}
		if(vm.count("user-network-graphml"))
		{
			std::ofstream file(vm["user-network-graphml"].as<std::string>());
			graphToGraphml(file, cache.GetUserGraph());
		}
		if(vm.count("user-network-graphviz"))
		{
			std::ofstream file(vm["user-network-graphviz"].as<std::string>());
			graphToGraphviz(file, cache.GetUserGraph());
		}

		if(vm.count("comment-network-gml"))
		{
			std::ofstream file(vm["comment-network-gml"].as<std::string>());
			graphToGml(file, cache.GetCommentGraph());
		}
		if(vm.count("comment-network-graphml"))
		{
			std::ofstream file(vm["comment-network-graphml"].as<std::string>());
			graphToGraphml(file, cache.GetCommentGraph());
		}
		if(vm.count("comment-network-graphviz"))
		{
			std::ofstream file(vm["comment-network-graphviz"].as<std::string>());
			graphToGraphviz(file, cache.GetCommentGraph());
		}

		if(vm.count("comment-list-csv"))
		{
			std::ofstream file(vm["comment-list-csv"].as<std::string>());
			listToCsv(file, parsedTalkPage);
		}

		if(vm.count("comment-list-human-readable"))
		{
			std::ofstream file(vm["comment-list-human-readable"].as<std::string>());
			listToHumanReadable(file, parsedTalkPage);
		}

		if(vm.count("comment-list-json"))
		{
			std::ofstream file(vm["comment-list-json"].as<std::string>());
			listToJson(file, parsedTalkPage);
		}

		if(vm.count("two-mode-network-gml"))
		{
			std::ofstream file(vm["two-mode-network-gml"].as<std::string>());
			graphToGml(file, cache.GetTwoModeGraph());
		}
		if(vm.count("two-mode-network-graphml"))
		{
			std::ofstream file(vm["two-mode-network-graphml"].as<std::string>());
			graphToGraphml(file, cache.GetTwoModeGraph());
		}
		if(vm.count("two-mode-network-graphviz"))
		{
			std::ofstream file(vm["two-mode-network-graphviz"].as<std::string>());
			graphToGraphviz(file, cache.GetTwoModeGraph());
		}
	}

}
