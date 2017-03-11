#include "xmlDumpParserWrapper.h"

#include "xmlDumpParsingHandler.h"
#include "../../../libs/wiki_xml_dump_xerces/src/parsers/parallelParser.hpp"
#include "../../../libs/wiki_xml_dump_xerces/src/parsers/singleCoreParser.hpp"
#include "../../../libs/wiki_xml_dump_xerces/src/handlers/wikiDumpHandlerProperties.hpp"

#include <boost/filesystem.hpp>
#include <iostream>

namespace Grawitas {
	void xml_dump_parsing(std::string input_xml_folder, std::string output_folder, const std::set<Format>& formats)
	{
		// scan for xml files in the input-folder
		std::vector<boost::filesystem::path> xmlFileList;
		for(auto dir_it = boost::filesystem::directory_iterator(input_xml_folder); dir_it != boost::filesystem::directory_iterator(); dir_it++)
		{
			if(!boost::filesystem::is_directory(dir_it->path()))
				xmlFileList.push_back(dir_it->path());
		}

		// std::cout << "-----------------------------------------------------------------------" << std::endl;
		// std::cout << "Found the following files: " << std::endl;
		std::vector<std::string> paths;
		for (auto el : xmlFileList) {
			// std::cout << el << std::endl;
			paths.push_back(el.string());
		}

		// init xerces
		xercesc::XMLPlatformUtils::Initialize();

		WikiXmlDumpXerces::WikiDumpHandlerProperties parser_properties;
		// if(vm.count("article-list-file")) 
		// {
			// std::ifstream article_list_file(vm["article-list-file"].as<std::string>());
			// std::set<std::string> article_list;
			// std::string line;
			// while(std::getline(article_list_file, line))
			// {
				// boost::trim(line);
				// article_list.insert(line);
			// }

			// parser_properties.TitleFilter = [article_list](const std::string& title) {
				// if(title.substr(0,5) != "Talk:")
					// return false;
				// if(article_list.find(title.substr(5,title.length()-5)) == article_list.end())
					// return false;

				// return true;
			// };
		// }
		// else
		// {
			parser_properties.TitleFilter = [](const std::string& title) {
				return title.substr(0,5) == "Talk:";
			};
		// }
		// parser_properties.ProgressCallback = std::bind(printProgress, pageCounts, "bla", std::placeholders::_1);

		// WikiXmlDumpXerces::ParallelParser<XmlDumpParsingHandler> parser([&vm](){ return XmlDumpParsingHandler(vm); }, parser_properties);

		XmlDumpParsingHandler handler(formats, output_folder);
		WikiXmlDumpXerces::SingleCoreParser parser(handler, parser_properties);
		parser.Run(paths.begin(), paths.end());

		xercesc::XMLPlatformUtils::Terminate();
	}
}
