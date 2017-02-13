#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <streambuf>

#include <boost/program_options.hpp>

// #include "parsers/cleaningSyntax.h"
// #include "parsers/commentParser.h"
// #include "parsers/talkPageParsing.h"
#include "models/graph.h"
#include "graphComputation/graphComputationCache.h"

#include "output/outputWrapper.h"
#include "output/stepTimer.h"
#include "parsers/talkPageParser.h"

namespace po = boost::program_options;

using namespace WikiTalkNet;
using namespace std;

int main(int argc, char** argv) {
	// TODO go through all parameters again
	
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "Produce help message.")

			// input
            ("input-talk-page", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("stdin-input-talk-page", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")

			// network output
            ("user-network-gml", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("user-network-graphml", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("user-network-graphviz", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")

            ("comment-network-gml", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("comment-network-graphml", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("comment-network-graphviz", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")

            ("two-mode-network-gml", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("two-mode-network-graphml", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("two-mode-network-graphviz", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")

			// list output
            ("comment-list-csv", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("comment-list-human-readable", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")
            ("comment-list-json", po::value<string>(), "Path to an input file containing a talk page in the wikipedia syntax.")

			// misc
            ("show-timings", po::bool_switch()->default_value(false), "Path to an input file containing a talk page in the wikipedia syntax.")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

	StepTimer timings;
	timings.startTiming("global", "Total");

    if (vm.count("help")) {
        cout << desc << endl;
        return 0;
    }

	timings.startTiming("reading_data", "Reading in file to string");
    // first get the input content in place, either directly from a wikiSyntax file or by extracting it from a html file

    if(!vm.count("input-talk-page") && !vm.count("stdin-input-talk-page")) {
        cout << "The parameter --input-talk-page was NOT specified. Please specify some form of input. For displaying a parameter description please use --help." << std::endl;
		return 1;
    } 
	
	timings.stopTiming("reading_data");


    // // prepare content by removing unimportant and incorrect stuff
    // removeNoWikiStuff(wikiSyntaxContent);
    // removeHtmlTags(wikiSyntaxContent);
    // replaceOutdentMarkers(wikiSyntaxContent);
    // removeWikiMetaCommands(wikiSyntaxContent);
	// timings.stopTiming("cleaning_data");

	// // boost::trim(wikiSyntaxContent);

    // // first split the syntax into paragraphs with their respective titles
	// timings.startTiming("parsing", "Parsing comments");
    // ParagraphWithHeader p({ HeaderDescriptor(), wikiSyntaxContent });
    // auto paragraphs = std::list<ParagraphWithHeader>{ p };
    // splitByHeader(paragraphs, 2);
    // splitByHeader(paragraphs, 3);

    // // split the paragraphs into comments, parse them and calculate their ids
	// ParsedTalkPage parsedTalkPage;
    // for(auto& paragraph : paragraphs)
    // {
        // unsigned commentId = 1;
        // auto list = parseLinesIntoComments(splitIntoComments(paragraph.second));
        // calculateIds(list,commentId);
		// parsedTalkPage.push_back({ paragraph.first, list });
    // }
	timings.startTiming("parsing", "Parsing comments");
	ParsedTalkPage parsedTalkPage;
	if(vm.count("input-talk-page"))
	{
		std::ifstream wiki_input_file(vm["input-talk-page"].as<string>());
		parsedTalkPage = parseTalkPage(wiki_input_file);	
	}
	else
	{
		parsedTalkPage = parseTalkPage(std::cin);	
	}

	std::size_t curId = 1;
	for (auto& sec : parsedTalkPage) {
		calculateIds(sec.second, curId);	
	}
	timings.stopTiming("parsing");

	timings.startTiming("output", "Output generation (include computation of graphs)");
	outputWrapper(vm, parsedTalkPage);
	timings.stopTiming("output");

	timings.stopTiming("global");

	if(vm.count("show-timings") && vm["show-timings"].as<bool>())
	{
		std::cout << std::endl << "--- Timings ---" << std::endl;
		timings.printTimings(std::cout);
		std::cout << std::endl;
	}

	return 0;
}
