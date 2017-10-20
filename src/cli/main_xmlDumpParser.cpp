#include <iostream>
#include <fstream>

#include "../../libs/cxxopts/include/cxxopts.hpp"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "helpers/stepTimer.h"
#include "output/formats.h"

#include "parsing/xmlDumpParserWrapper.h"

namespace fs = boost::filesystem;

using namespace Grawitas;
using namespace std;

int main(int argc, char** argv) 
{
	StepTimer timings;
	timings.startTiming("global", "Total");

    cxxopts::Options options("grawitas_cli_xml", "Grawitas CLI xml parser.");
	options.add_options()
		("help", "Produce help message.")
		("input-xml-folder", "The folder that should be scanned for .xml files that are part of a dump of Wikipedia. WARNING: This folder should only contain readable .xml files!", cxxopts::value<string>())
		("output-folder", "The folder in which the results (articlesWithDates.txt, categories.txt, redirects.txt) should be stored.", cxxopts::value<string>())
		("article-list-file", "Path to a file containing the list of articles for which talk pages should be parsed.", cxxopts::value<string>())

		// network output
		("user-network-gml", "Flag indicating the output of the user network of the talk page in an .gml-format.")
		("user-network-graphml", "Flag indicating the output of the user network of the talk page in an .graphml-format.")
		("user-network-graphviz", "Flag indicating the output of the user network of the talk page in an .graphviz-format.")

		("comment-network-gml", "Flag indicating the output of the comment network of the talk page in an .gml-format.")
		("comment-network-graphml", "Flag indicating the output of the comment network of the talk page in an .graphml-format.")
		("comment-network-graphviz", "Flag indicating the output of the comment network of the talk page in an .graphviz-format.")

		("two-mode-network-gml", "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .gml-format.")
		("two-mode-network-graphml", "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .graphml-format.")
		("two-mode-network-graphviz", "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .graphviz-format.")

		// list output
		("comment-list-csv", "Flag indicating the output of a list of comments with user, date, and parent comment in .csv-format.")
		("comment-list-human-readable", "Flag indicating the output of a list of comments with user, date, and parent comment in an human readable format.")
		("comment-list-json", "Flag indicating the output of a list of comments with user, date, and parent comment in a .json-format.")

		// misc
		("show-timings", "Show the timings for the different parsing steps.")
		;

	options.parse(argc, argv);

	// display help if --help was specified
	if (options.count("help")) {
		cout << options.help() << endl;
		return 0;
	}

	if(!options.count("input-xml-folder") || !options.count("output-folder"))
	{
		std::cerr << "Please specify the parameters --input-xml-folder and --output-folder." << std::endl;
		return 1;
	}

	const fs::path inputFolder(options["input-xml-folder"].as<std::string>());
	const fs::path outputFolder(options["output-folder"].as<std::string>());

	if(!fs::is_directory(inputFolder))
	{
		std::cerr << "Parameter --input-xml-folder is no folder." << std::endl;
		return 1;
	}

	if(!fs::is_directory(outputFolder))
	{
		std::cerr << "Parameter --ouput-folder is no folder, creating it..." << std::endl;
		fs::create_directory(outputFolder);
	}


	std::set<Format> formats;
	for (auto form_parameter : FormatParameterStrings) 
		if(options[form_parameter].as<bool>())
			formats.insert(parameter_to_format(form_parameter));

	xml_dump_parsing(inputFolder.string(), outputFolder.string(), formats);

	// std::cout << "-----------------------------------------------------------------------" << std::endl;
	// std::cout << "Status: " << std::endl;
	// std::cout << std::left << std::setw(40) << "Total number of articles: " << totalArticleNumber << std::endl;
	// std::cout << std::left << std::setw(40) << "Articles with extractable dates: " << articlesWithDates.size() << std::endl;
	// std::cout << std::left << std::setw(40) << "Categories extracted: " << categories.size() << std::endl;
	// std::cout << std::left << std::setw(40) << "Redirects extracted: " << redirects.size() << std::endl;

	// // output timings
	// std::cout << "-----------------------------------------------------------------------" << std::endl;
	// std::cout << "Timings: " << std::endl << std::endl;
	// for(auto timing : timings)
	// std::cout << std::left << std::setw(50) << timing.first + ": " << timingToReadable(timing.second) << std::endl;

	// std::cout << "-----------------------------------------------------------------------" << std::endl;

	return 0;
}
