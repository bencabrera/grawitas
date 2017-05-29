#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include "output/outputWrapper.h"
#include "helpers/stepTimer.h"
#include "parsing/coreTalkPageParsing.h"
#include "crawlerThread.h"

namespace po = boost::program_options;

using namespace Grawitas;
using namespace std;

int main(int argc, char** argv) 
{
    po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Produces this help message.")

		("talk-page-list-file,i", po::value<string>(), "Path to an input file containing a talk page in the Wikipedia syntax.")
		("output-folder,o", po::value<string>(), "Path to an input file containing a talk page in the Wikipedia syntax.")

		// network output
		("user-network-gml", po::bool_switch()->default_value(false), "Path for the potential output file containing the user network of the talk page in an .gml-format.")
		("user-network-graphml", po::bool_switch()->default_value(false), "Path for the potential output file containing the user network of the talk page in an .graphml-format.")
		("user-network-graphviz", po::bool_switch()->default_value(false), "Path for the potential output file containing the user network of the talk page in an .graphviz-format.")

		("comment-network-gml", po::bool_switch()->default_value(false), "Path for the potential output file containing the comment network of the talk page in an .gml-format.")
		("comment-network-graphml", po::bool_switch()->default_value(false), "Path for the potential output file containing the comment network of the talk page in an .graphml-format.")
		("comment-network-graphviz", po::bool_switch()->default_value(false), "Path for the potential output file containing the comment network of the talk page in an .graphviz-format.")

		("two-mode-network-gml", po::bool_switch()->default_value(false), "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .gml-format.")
		("two-mode-network-graphml", po::bool_switch()->default_value(false), "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .graphml-format.")
		("two-mode-network-graphviz", po::bool_switch()->default_value(false), "Path for the potential output file containing the talk page network as a two mode network consisting of user and comment vertices in an .graphviz-format.")

		// list output
		("comment-list-csv", po::bool_switch()->default_value(false), "Path for the potential output file containing the list of comments with user, date, and parent comment in .csv-format.")
		("comment-list-human-readable", po::bool_switch()->default_value(false), "Path for the potential output file containing the list of comments with user, date, and parent comment in an human readable format.")
		("comment-list-json", po::bool_switch()->default_value(false), "Path for the potential output file containing the list of comments with user, date, and parent comment in a .json-format.")

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



    timings.stopTiming(("global"));

	return 0;
}
