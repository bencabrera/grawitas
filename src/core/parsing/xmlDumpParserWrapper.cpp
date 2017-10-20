#include "xmlDumpParserWrapper.h"

#include "xmlDumpParsingHandler.h"
#include "../../../libs/wiki_xml_dump_xerces/src/parsers/singleCoreParser.hpp"
#include "../../../libs/wiki_xml_dump_xerces/src/handlers/wikiDumpHandlerProperties.hpp"

#include <iostream>

namespace Grawitas {
	void xml_dump_parsing(std::string input_xml_file, std::string output_folder, const std::set<Format>& formats, std::vector<std::function<void(std::string)>> report_status_callbacks)
	{
		// init xerces
		xercesc::XMLPlatformUtils::Initialize();

		WikiXmlDumpXerces::WikiDumpHandlerProperties parser_properties;
		parser_properties.TitleFilter = [](const std::string& title) {
			return title.substr(0,5) == "Talk:";
		};

		XmlDumpParsingHandler handler(formats, output_folder);
		handler.report_status_callbacks = report_status_callbacks;
		WikiXmlDumpXerces::SingleCoreParser parser(handler, parser_properties);
		parser.Run(input_xml_file);

		xercesc::XMLPlatformUtils::Terminate();
	}
}
