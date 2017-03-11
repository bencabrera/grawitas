#include "xmlDumpParsingHandler.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "coreTalkPageParsing.h"
#include "../output/outputWrapper.h"

namespace Grawitas {

	std::map<Format, std::string> XmlDumpParsingHandler::format_file_extensions = {
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

	XmlDumpParsingHandler::XmlDumpParsingHandler(const std::set<Format>& formats, std::string output_folder)
		:_formats(formats),
		_output_folder(output_folder)
	{}

	void XmlDumpParsingHandler::HandleArticle(const WikiXmlDumpXerces::WikiPageData& data)
	{
		std::string title_filename = safeEncodeTitleToFilename(data.MetaData.at("title"));
		
		auto parsed_talk_page = parseTalkPage(data.Content);		

		std::size_t curId = 1;
		for (auto& sec : parsed_talk_page) {
			calculateIds(sec.second, curId);	
		}

		std::map<Format, std::string> formats_with_paths;
		for (auto format : _formats) 
			formats_with_paths.insert({ format, (_output_folder / (title_filename + format_file_extensions.at(format))).string() });

		output_in_formats_to_files(formats_with_paths, parsed_talk_page);
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
