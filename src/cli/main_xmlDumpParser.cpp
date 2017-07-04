#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "../core/helpers/stepTimer.h"
#include "../core/output/formats.h"

#include "../core/parsing/xmlDumpParserWrapper.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace Grawitas;
using namespace std;

int main(int argc, char** argv) 
{
	StepTimer timings;
	timings.startTiming("global", "Total");

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Produce help message.")
		("input-xml-folder", po::value<string>(), "The folder that should be scanned for .xml files that are part of a dump of Wikipedia.")
		("output-folder", po::value<string>(), "The folder in which the results (articlesWithDates.txt, categories.txt, redirects.txt) should be stored.")
		("article-list-file", po::value<string>(), "Path to a file containing the list of articles for which talk pages should be parsed.")

		// network output
		("user-network-gml", po::bool_switch()->default_value(false), "Flag indicating the output of the user network of the talk page in an .gml-format.")
		("user-network-graphml", po::bool_switch()->default_value(false), "Flag indicating the output of the user network of the talk page in an .graphml-format.")
		("user-network-graphviz", po::bool_switch()->default_value(false), "Flag indicating the output of the user network of the talk page in an .graphviz-format.")

		("comment-network-gml", po::bool_switch()->default_value(false), "Flag indicating the output of the comment network of the talk page in an .gml-format.")
		("comment-network-graphml", po::bool_switch()->default_value(false), "Flag indicating the output of the comment network of the talk page in an .graphml-format.")
		("comment-network-graphviz", po::bool_switch()->default_value(false), "Flag indicating the output of the comment network of the talk page in an .graphviz-format.")

		("two-mode-network-gml", po::bool_switch()->default_value(false), "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .gml-format.")
		("two-mode-network-graphml", po::bool_switch()->default_value(false), "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .graphml-format.")
		("two-mode-network-graphviz", po::bool_switch()->default_value(false), "Flag indicating the output of the talk page network as a two mode network consisting of user and comment vertices in an .graphviz-format.")

		// list output
		("comment-list-csv", po::bool_switch()->default_value(false), "Flag indicating the output of a list of comments with user, date, and parent comment in .csv-format.")
		("comment-list-human-readable", po::bool_switch()->default_value(false), "Flag indicating the output of a list of comments with user, date, and parent comment in an human readable format.")
		("comment-list-json", po::bool_switch()->default_value(false), "Flag indicating the output of a list of comments with user, date, and parent comment in a .json-format.")

		// misc
		("show-timings", po::bool_switch()->default_value(false), "Show the timings for the different parsing steps.")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// display help if --help was specified
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}

	if(!vm.count("input-xml-folder") || !vm.count("output-folder"))
	{
		std::cerr << "Please specify the parameters --input-xml-folder and --output-folder." << std::endl;
		return 1;
	}

	const fs::path inputFolder(vm["input-xml-folder"].as<std::string>());
	const fs::path outputFolder(vm["output-folder"].as<std::string>());

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
		if(vm[form_parameter].as<bool>())
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
