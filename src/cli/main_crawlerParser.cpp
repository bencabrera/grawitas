#include <iostream>
#include <fstream>

#include "../../libs/cxxopts/include/cxxopts.hpp"

#include "../output/outputWrapper.h"
#include "../output/formats.h"
#include "../talkPageParser/parsing.h"
#include "../talkPageParser/models.h"
#include "../httpCrawler/crawling.h"

#include "../output/outputHelpers.h"
#include "../misc/readLinesFromFile.h"
#include <boost/algorithm/string/trim.hpp>

using namespace Grawitas;
using namespace std;

std::set<Grawitas::Format> formats_from_parameters(const cxxopts::Options& arguments)
{
	std::set<Grawitas::Format> formats;

	for (auto form_parameter : Grawitas::FormatParameterStrings) 
		if(arguments.count(form_parameter) > 0 && arguments[form_parameter].as<bool>())
			formats.insert(Grawitas::parameter_to_format(form_parameter));

	return formats;
}

int main(int argc, char** argv)
{
	cxxopts::Options options("grawitas_cli_crawler", "Downloads talk pages from Wikipedia and parses them. It also automatically retrieves archived talk pages.");
	options.add_options()
		("h,help", "Produces this help message.")

		("i,talk-page-list-file", "Path to an input file containing titles of Wikipedia articles.", cxxopts::value<string>())
		("o,output-folder", "Path to an input file containing a talk page in the Wikipedia syntax.", cxxopts::value<string>())

		// network output
		("user-network-gml", "Flag for exporting as user network (GML).")
		("user-network-graphml", "Flag for exporting the user network (GraphML).")
		("user-network-graphviz", "Flag for exporting the user network (GraphViz).")

		("comment-network-gml", "Flag for exporting the comment network (GML).")
		("comment-network-graphml", "Flag for exporting the comment network (GraphML).")
		("comment-network-graphviz", "Flag for exporting the comment network (GraphViz).")

		("two-mode-network-gml", "Flag for exporting the two-mode network (GML).")
		("two-mode-network-graphml", "Flag for exporting the two-mode network (GraphML).")
		("two-mode-network-graphviz", "Flag for exporting the two-mode network (GraphViz).")

		// list output
		("comment-list-csv", "Flag for exporting the list of comments (csv).")
		("comment-list-human-readable", "Flag for exporting the list of comments (human readable).")
		("comment-list-json", "Flag for exporting the list of comments (json).")

		("keep-raw-talk-pages", "Flag for keeping the raw crawled talk pages.")
    
        ("split-by-date", "Flag for splitting talk pages into two seperate files by some date")
		// misc
		("show-timings", "Show the timings for the different steps.")
		;
	options.positional_help("<talk-page-list-file> <output-folder>");

	try {
		options.parse_positional(std::vector<std::string>{"talk-page-list-file","output-folder"});
		options.parse(argc, argv);

		// show help and exit program if --help is set
		if (options.count("help")) {
			cout << options.help() << endl;
			return 0;
		}

		const auto formats = formats_from_parameters(options);
		if(!options.count("talk-page-list-file") || !options.count("output-folder")) 
			throw std::invalid_argument("Talk page list file or output folder not specified.");

		const auto input_file_path = options["talk-page-list-file"].as<std::string>();
		const auto output_folder = normalize_folder_path(options["output-folder"].as<std::string>());

		std::ifstream input_file(input_file_path);
		if (!input_file.is_open())
			throw std::invalid_argument("File containing article titles could not be opened.");

		const auto titles = read_lines_from_file(input_file);

		AdditionalCrawlerOptions crawler_options;
		crawler_options.keep_raw_talk_pages = options.count("keep-raw-talk-pages");
        crawler_options.split_by_date = options.count("split-by-date"); 
		crawler_options.status_callback = [](const std::string& msg) { std::cout << msg << std::endl; };
		crawling(titles, output_folder, formats, crawler_options);
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
