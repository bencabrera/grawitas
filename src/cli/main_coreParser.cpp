#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include "output/outputWrapper.h"
#include "helpers/stepTimer.h"
#include "parsing/coreTalkPageParsing.h"

namespace po = boost::program_options;

using namespace Grawitas;
using namespace std;

int main(int argc, char** argv) 
{
    po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Produces this help message.")

		// input
		("input-talk-page-file", po::value<string>(), "Path to an input file containing a talk page in the Wikipedia syntax.")

		// network output
		("user-network-gml", po::value<string>(), "Path for the potential output file containing the user network of the talk page in an .gml-format.")
		("user-network-graphml", po::value<string>(), "Path for the potential output file containing the user network of the talk page in an .graphml-format.")
		("user-network-graphviz", po::value<string>(), "Path for the potential output file containing the user network of the talk page in an .graphviz-format.")

		("comment-network-gml", po::value<string>(), "Path for the potential output file containing the comment network of the talk page in an .gml-format.")
		("comment-network-graphml", po::value<string>(), "Path for the potential output file containing the comment network of the talk page in an .graphml-format.")
		("comment-network-graphviz", po::value<string>(), "Path for the potential output file containing the comment network of the talk page in an .graphviz-format.")

		("two-mode-network-gml", po::value<string>(), "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .gml-format.")
		("two-mode-network-graphml", po::value<string>(), "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .graphml-format.")
		("two-mode-network-graphviz", po::value<string>(), "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .graphviz-format.")

		// list output
		("comment-list-csv", po::value<string>(), "Path for the potential output file containing the list of comments with user, date, and parent comment in .csv-format.")
		("comment-list-human-readable", po::value<string>(), "Path for the potential output file containing the list of comments with user, date, and parent comment in an human readable format.")
		("comment-list-json", po::value<string>(), "Path for the potential output file containing the list of comments with user, date, and parent comment in a .json-format.")

		// misc
		("show-timings", po::bool_switch()->default_value(false), "Show the timings for the different parsing steps.")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// show help and exit program if --help is set
	if (vm.count("help")) {
		cout << desc << endl;
		return 0;
	}

	// start taking timings from here on; StepTimer is a helper class for that
	StepTimer timings;
	timings.startTiming("global", "Total");

	if(!vm.count("input-talk-page-file")) {
		cout << "The parameter --input-talk-page-file was NOT specified. Please specify the input file. To display a parameter description please use --help." << std::endl;
		return 1;
	} 

	// the actual parsing part of the program; starts by reading from a file
	timings.startTiming("parsing", "Splitting talk page into comments");
	ParsedTalkPage parsedTalkPage;
	if(vm.count("input-talk-page-file"))
	{
		std::ifstream wiki_input_file(vm["input-talk-page-file"].as<string>());
		parsedTalkPage = parse_talk_page(wiki_input_file);	
	}
	timings.stopTiming("parsing");

	// generate the output in the different specified formats
	timings.startTiming("output", "Generation of output");
	std::map<Format, std::string> formats;
	for (auto form_parameter : FormatParameterStrings) {
		if(vm.count(form_parameter))
			formats.insert({ parameter_to_format(form_parameter), vm.at(form_parameter).as<std::string>() });
	}
	output_in_formats_to_files(formats, parsedTalkPage);
	timings.stopTiming("output");

	timings.stopTiming("global");

	// if --show-timings was set, show the timings of the different steps
	if(vm.count("show-timings") && vm["show-timings"].as<bool>())
	{
		std::cout << std::endl << "--- Timings ---" << std::endl;
		timings.printTimings(std::cout);
		std::cout << std::endl;
	}

	return 0;
}
