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

#include "../networks/graphComputationCache.h"

namespace Grawitas {

	void output_in_format_to_stream(std::ostream& ostr, Format format, const ParsedTalkPage& parsedTalkPage, bool beautify)
	{
		auto cache = GraphComputationCache(parsedTalkPage);

		if(format == USER_NETWORK_GML)
			graphToGml(ostr, cache.GetUserGraph());
		if(format == USER_NETWORK_GRAPHML)
			graphToGraphml(ostr, cache.GetUserGraph());
		if(format == USER_NETWORK_GRAPHVIZ)
			graphToGraphviz(ostr, cache.GetUserGraph());

		if(format == COMMENT_NETWORK_GML)
			graphToGml(ostr, cache.GetCommentGraph());
		if(format == COMMENT_NETWORK_GRAPHML)
			graphToGraphml(ostr, cache.GetCommentGraph());
		if(format == COMMENT_NETWORK_GRAPHVIZ)
			graphToGraphviz(ostr, cache.GetCommentGraph());

		if(format == COMMENT_LIST_CSV)
			listToCsv(ostr, parsedTalkPage);
		if(format == COMMENT_LIST_HUMAN_READABLE)
			listToHumanReadable(ostr, parsedTalkPage);
		if(format == COMMENT_LIST_JSON)
			listToJson(ostr, parsedTalkPage, {"id", "parent_id", "user", "date", "text"}, beautify);

		if(format == TWO_MODE_NETWORK_GML)
			graphToGml(ostr, cache.GetTwoModeGraph());
		if(format == TWO_MODE_NETWORK_GRAPHML)
			graphToGraphml(ostr, cache.GetTwoModeGraph());
		if(format == TWO_MODE_NETWORK_GRAPHVIZ)
			graphToGraphviz(ostr, cache.GetTwoModeGraph());
	}

	void output_in_formats_to_files(const std::map<Format, std::string>& formats, const ParsedTalkPage& parsedTalkPage)
	{
		auto cache = GraphComputationCache(parsedTalkPage);

		if(formats.count(USER_NETWORK_GML))
		{
			std::ofstream file(formats.at(USER_NETWORK_GML));
			graphToGml(file, cache.GetUserGraph());
		}
		if(formats.count(USER_NETWORK_GRAPHML))
		{
			std::ofstream file(formats.at(USER_NETWORK_GRAPHML));
			graphToGraphml(file, cache.GetUserGraph());
		}
		if(formats.count(USER_NETWORK_GRAPHVIZ))
		{
			std::ofstream file(formats.at(USER_NETWORK_GRAPHVIZ));
			graphToGraphviz(file, cache.GetUserGraph());
		}

		if(formats.count(COMMENT_NETWORK_GML))
		{
			std::ofstream file(formats.at(COMMENT_NETWORK_GML));
			graphToGml(file, cache.GetCommentGraph());
		}
		if(formats.count(COMMENT_NETWORK_GRAPHML))
		{
			std::ofstream file(formats.at(COMMENT_NETWORK_GRAPHML));
			graphToGraphml(file, cache.GetCommentGraph());
		}
		if(formats.count(COMMENT_NETWORK_GRAPHVIZ))
		{
			std::ofstream file(formats.at(COMMENT_NETWORK_GRAPHVIZ));
			graphToGraphviz(file, cache.GetCommentGraph());
		}

		if(formats.count(COMMENT_LIST_CSV))
		{
			std::ofstream file(formats.at(COMMENT_LIST_CSV));
			listToCsv(file, parsedTalkPage);
		}

		if(formats.count(COMMENT_LIST_HUMAN_READABLE))
		{
			std::ofstream file(formats.at(COMMENT_LIST_HUMAN_READABLE));
			listToHumanReadable(file, parsedTalkPage);
		}

		if(formats.count(COMMENT_LIST_JSON))
		{
			std::ofstream file(formats.at(COMMENT_LIST_JSON));
			listToJson(file, parsedTalkPage,{"id", "parent_id", "user", "date", "text"});
		}

		if(formats.count(TWO_MODE_NETWORK_GML))
		{
			std::ofstream file(formats.at(TWO_MODE_NETWORK_GML));
			graphToGml(file, cache.GetTwoModeGraph());
		}
		if(formats.count(TWO_MODE_NETWORK_GRAPHML))
		{
			std::ofstream file(formats.at(TWO_MODE_NETWORK_GRAPHML));
			graphToGraphml(file, cache.GetTwoModeGraph());
		}
		if(formats.count(TWO_MODE_NETWORK_GRAPHVIZ))
		{
			std::ofstream file(formats.at(TWO_MODE_NETWORK_GRAPHVIZ));
			graphToGraphviz(file, cache.GetTwoModeGraph());
		}
	}

}
