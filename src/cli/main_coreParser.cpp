#include <iostream>
#include <fstream>

#include "../../libs/cxxopts/include/cxxopts.hpp"

#include "../core/output/outputWrapper.h"
#include "../core/helpers/stepTimer.h"
#include "../core/parsing/coreTalkPageParsing.h"

using namespace Grawitas;
using namespace std;

int main(int argc, char** argv) 
{
    cxxopts::Options options("grawitas_cli_core", "Grawitas CLI core parser.");
	options.add_options()
		("help", "Produces this help message.")

		// input
		("i,input-talk-page-file", "Path to an input file containing a talk page in the Wikipedia syntax.", cxxopts::value<string>())

		// network output
		("user-network-gml", "Path for the potential output file containing the user network of the talk page in an .gml-format.", cxxopts::value<string>())
		("user-network-graphml", "Path for the potential output file containing the user network of the talk page in an .graphml-format.", cxxopts::value<string>())
		("user-network-graphviz", "Path for the potential output file containing the user network of the talk page in an .graphviz-format.", cxxopts::value<string>())

		("comment-network-gml", "Path for the potential output file containing the comment network of the talk page in an .gml-format.", cxxopts::value<string>())
		("comment-network-graphml", "Path for the potential output file containing the comment network of the talk page in an .graphml-format.", cxxopts::value<string>())
		("comment-network-graphviz", "Path for the potential output file containing the comment network of the talk page in an .graphviz-format.", cxxopts::value<string>())

		("two-mode-network-gml", "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .gml-format.", cxxopts::value<string>())
		("two-mode-network-graphml", "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .graphml-format.", cxxopts::value<string>())
		("two-mode-network-graphviz", "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .graphviz-format.", cxxopts::value<string>())

		// list output
		("comment-list-csv", "Path for the potential output file containing the list of comments with user, date, and parent comment in .csv-format.", cxxopts::value<string>())
		("comment-list-human-readable", "Path for the potential output file containing the list of comments with user, date, and parent comment in an human readable format.", cxxopts::value<string>())
		("comment-list-json", "Path for the potential output file containing the list of comments with user, date, and parent comment in a .json-format.", cxxopts::value<string>())

		// misc
		("show-timings", "Show the timings for the different parsing steps.")
		;

	options.parse(argc, argv);

	// show help and exit program if --help is set
	if (options.count("help")) {
		cout << options.help() << endl;
		return 0;
	}

	// start taking timings from here on; StepTimer is a helper class for that
	StepTimer timings;
	timings.startTiming("global", "Total");

	if(!options.count("input-talk-page-file")) {
		cout << "The parameter --input-talk-page-file was NOT specified. Please specify the input file. To display a parameter description please use --help." << std::endl;
		return 1;
	} 


	try {
		// the actual parsing part of the program; starts by reading from a file
		timings.startTiming("parsing", "Splitting talk page into comments");
		ParsedTalkPage parsedTalkPage;
		if(options.count("input-talk-page-file"))
		{
			std::ifstream wiki_input_file(options["input-talk-page-file"].as<string>());
			if (!wiki_input_file.is_open())
			{
				std::cerr << "Input talk page file could not be opened. Aborting." << std::endl;
				return 1;
			}
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
	}
	catch(const std::exception& exception) {
		std::cerr << "--------------------------------------------------" << std::endl;
		std::cerr << "FATAL ERROR: The application terminated with an exception:" << std::endl;
		std::cerr << exception.what() << std::endl;
		std::cerr << "--------------------------------------------------" << std::endl;
	}

	timings.stopTiming("global");

	// if --show-timings was set, show the timings of the different steps
	if(options.count("show-timings") && options["show-timings"].as<bool>())
	{
		std::cout << std::endl << "--- Timings ---" << std::endl;
		timings.printTimings(std::cout);
		std::cout << std::endl;
	}

	return 0;
}
