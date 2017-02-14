#include "xmlDumpParsingHandler.h"

#include <sstream>
#include <iomanip>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "coreTalkPageParsing.h"
#include "../graphComputation/graphComputationCache.h"

#include "../output/listCsvOutput.h"
#include "../output/graphGmlOutput.h"
#include "../output/graphGraphmlOutput.h"
#include "../output/graphGraphvizOutput.h"
#include "../output/listCsvOutput.h"
#include "../output/listHumanReadableOutput.h"
#include "../output/listJsonOutput.h"

namespace Grawitas {

	XmlDumpParsingHandler::XmlDumpParsingHandler(const boost::program_options::variables_map& vm)
		:_programm_options(vm),
		_output_folder(vm["output-folder"].as<std::string>())
	{}

	void XmlDumpParsingHandler::HandleArticle(const WikiXmlDumpXerces::WikiPageData& data)
	{
		std::string title_filename = safeEncodeTitleToFilename(data.MetaData.at("title"));

		auto parsedTalkPage = parseTalkPage(data.Content);		

		std::size_t curId = 1;
		for (auto& sec : parsedTalkPage) {
			calculateIds(sec.second, curId);	
		}

		auto cache = GraphComputationCache(parsedTalkPage);

		if(_programm_options.count("user-network-gml"))
		{
			std::string filename = title_filename + ".user_network.gml";
			std::ofstream file((_output_folder / filename).string());
			graphToGml(file, cache.GetUserGraph());
		}
		if(_programm_options.count("user-network-graphml"))
		{
			std::string filename = title_filename + ".user_network.graphml";
			std::ofstream file((_output_folder / filename).string());
			graphToGraphml(file, cache.GetUserGraph());
		}
		if(_programm_options.count("user-network-graphviz"))
		{
			std::string filename = title_filename + ".user_network.dot";
			std::ofstream file((_output_folder / filename).string());
			graphToGraphviz(file, cache.GetUserGraph());
		}

		if(_programm_options.count("comment-network-gml"))
		{
			std::string filename = title_filename + ".comment_network.gml";
			std::ofstream file((_output_folder / filename).string());
			graphToGml(file, cache.GetCommentGraph());
		}
		if(_programm_options.count("comment-network-graphml"))
		{
			std::string filename = title_filename + ".comment_network.graphml";
			std::ofstream file((_output_folder / filename).string());
			graphToGraphml(file, cache.GetCommentGraph());
		}
		if(_programm_options.count("comment-network-graphviz"))
		{
			std::string filename = title_filename + ".comment_network.dot";
			std::ofstream file((_output_folder / filename).string());
			graphToGraphviz(file, cache.GetCommentGraph());
		}

		if(_programm_options.count("comment-list-csv"))
		{
			std::string filename = title_filename + ".comment_list.csv";
			std::ofstream file((_output_folder / filename).string());
			listToCsv(file, parsedTalkPage);
		}

		if(_programm_options.count("comment-list-human-readable"))
		{
			std::string filename = title_filename + ".comment_list.txt";
			std::ofstream file((_output_folder / filename).string());
			listToHumanReadable(file, parsedTalkPage);
		}

		if(_programm_options.count("comment-list-json"))
		{
			std::string filename = title_filename + ".comment_list.json";
			std::ofstream file((_output_folder / filename).string());
			listToJson(file, parsedTalkPage);
		}

		if(_programm_options.count("two-mode-network-gml"))
		{
			std::string filename = title_filename + ".two_mode_network.gml";
			std::ofstream file((_output_folder / filename).string());
			graphToGml(file, cache.GetTwoModeGraph());
		}
		if(_programm_options.count("two-mode-network-graphml"))
		{
			std::string filename = title_filename + ".two_mode_network.graphml";
			std::ofstream file((_output_folder / filename).string());
			graphToGraphml(file, cache.GetTwoModeGraph());
		}
		if(_programm_options.count("two-mode-network-graphviz"))
		{
			std::string filename = title_filename + ".two_mode_network.dot";
			std::ofstream file((_output_folder / filename).string());
			graphToGraphviz(file, cache.GetTwoModeGraph());
		}
	}

	std::string XmlDumpParsingHandler::safeEncodeTitleToFilename(const std::string& title) const
	{
		std::string str = title.substr(5, title.length()-5);
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
