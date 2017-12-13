#include <iostream>
#include <fstream>

#include "../../libs/cxxopts/include/cxxopts.hpp"

#include "../output/outputWrapper.h"
#include "../misc/stepTimer.h"
#include "../talkPageParser/parsing.h"

using namespace Grawitas;

int main(int argc, char** argv) 
{
	cxxopts::Options options("grawitas_cli_core", "Parses a Wikipedia talk page into its seperate comments. The raw talk page has to be provided as a file by the user as opposed to e.g. the crawler where the pages are automatically obtained via the http api of Wikipedia.");
	options.add_options()
		("help", "Produces this help message.")

		// input
		("i,input-talk-page-file", "Talk page file to be parsed.", cxxopts::value<std::string>())

		// network output
		("user-network-gml", "Output file for user network (GML).", cxxopts::value<std::string>())
		("user-network-graphml", "Output file for user network (GraphML).", cxxopts::value<std::string>())
		("user-network-graphviz", "Output file for user network (GraphViz).", cxxopts::value<std::string>())

		("comment-network-gml", "Output file for comment network (GML).", cxxopts::value<std::string>())
		("comment-network-graphml", "Output file for comment network (GraphML).", cxxopts::value<std::string>())
		("comment-network-graphviz", "Output file for comment network (GraphViz).", cxxopts::value<std::string>())

		("two-mode-network-gml", "Output file for two-mode user-/comment network (GML).", cxxopts::value<std::string>())
		("two-mode-network-graphml", "Output file for two-mode user-/comment network (GraphML).", cxxopts::value<std::string>())
		("two-mode-network-graphviz", "Output file for two-mode user-/comment network (GraphViz).", cxxopts::value<std::string>())

		// list output
		("comment-list-csv", "Output file for comment list (csv).", cxxopts::value<std::string>())
		("comment-list-human-readable", "Output file for comment list (human readable).", cxxopts::value<std::string>())
		("comment-list-json", "Output file for comment list (json).", cxxopts::value<std::string>())

		// misc
		("show-timings", "Show the timings for the different parsing steps.")
		;
	options.positional_help("<input-talk-page-file>");

	try {
		options.parse_positional(std::vector<std::string>{"input-talk-page-file"});
		options.parse(argc, argv);

		// show help and exit program if --help is set
		if (options.count("help")) {
			std::cout << options.help() << std::endl;
			return 0;
		}

		// start taking timings from here on; StepTimer is a helper class for that
		StepTimer timings;
		timings.startTiming("global", "Total");

		if(!options.count("input-talk-page-file")) 
			throw std::invalid_argument("Input talk page file not specified.");

		// the actual parsing part of the program; starts by reading from a file
		timings.startTiming("parsing", "Splitting talk page into comments");
		ParsedTalkPage parsedTalkPage;
		if(options.count("input-talk-page-file"))
		{
			std::ifstream wiki_input_file(options["input-talk-page-file"].as<std::string>());
			if (!wiki_input_file.is_open())
				throw std::invalid_argument("Could not open input talk page file.");
			parsedTalkPage = parse_talk_page(wiki_input_file);	
		}
		timings.stopTiming("parsing");

		// generate the output in the different specified formats
		timings.startTiming("output", "Generation of output");
		std::map<Format, std::string> formats;
		for (auto form_parameter : FormatParameterStrings) {
			if(options.count(form_parameter))
				formats.insert({ parameter_to_format(form_parameter), options[form_parameter].as<std::string>() });
		}
		output_in_formats_to_files(formats, parsedTalkPage);
		timings.stopTiming("output");
		timings.stopTiming("global");

		// if --show-timings was set, show the timings of the different steps
		if(options.count("show-timings") && options["show-timings"].as<bool>())
		{
			std::cout << std::endl << "--- Timings ---" << std::endl;
			timings.printTimings(std::cout);
			std::cout << std::endl;
		}
	}
	catch(const std::invalid_argument& e) {
		std::cerr << "ABORTING. An argument error appeared: " << e.what() << std::endl << std::endl;
		std::cerr << "Try -h or --help for more information" << std::endl;
		return 1;
	}
	catch(const std::exception& e) {
		std::cerr << "ABORTING. The application terminated with an exception: " << std::endl << e.what() << std::endl << std::endl;
		return 1;
	}

	return 0;
}
