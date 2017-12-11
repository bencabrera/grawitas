#include <iostream>
#include <fstream>

#include "../../libs/cxxopts/include/cxxopts.hpp"
#include <boost/algorithm/string/trim.hpp>

#include "../misc/stepTimer.h"
#include "../misc/readLinesFromFile.h"
#include "../output/formats.h"

#include "../xmlDumpCrawler/xmlDumpParsing.h"
#include "../output/outputHelpers.h"

#include "../xmlDumpCrawler/xmlToSqliteHandler.h"
#include "../../libs/wiki_xml_dump_xerces/src/parsers/singleCoreParser.hpp"
#include "../../libs/wiki_xml_dump_xerces/src/handlers/wikiDumpHandlerProperties.hpp"

using namespace Grawitas;
using namespace std;

namespace {
	int callback(void* /*NotUsed*/, int /*argc*/, char ** /*argv*/, char ** /*azColName*/) {
		return 0;
	}

	void clear_sqlite_db(const std::string& output_sqlite_filepath)
	{
		// Open database
		sqlite3* sqlite_db;
		char* zErrMsg = 0;
		auto rc = sqlite3_open(output_sqlite_filepath.c_str(), &sqlite_db);

		if(rc) 
		{
			std::string msg = sqlite3_errmsg(sqlite_db);
			sqlite3_close(sqlite_db);
			throw std::logic_error("Can't open database: %s\n" + msg);
		}

		std::string query = "DROP TABLE IF EXISTS talk_page;";

		// Execute SQL statement
		rc = sqlite3_exec(sqlite_db, query.c_str(), callback, 0, &zErrMsg);

		if(rc != SQLITE_OK){
			std::string msg = zErrMsg;
			sqlite3_free(zErrMsg);
			sqlite3_close(sqlite_db);
			throw std::logic_error("SQL error: %s\n" + msg);
		} 
		sqlite3_close(sqlite_db);
	}
}

int main(int argc, char** argv) 
{
	StepTimer timings;
	timings.startTiming("global", "Total");

    cxxopts::Options options("grawitas_cli_xml", "Grawitas CLI xml parser.");
	options.add_options()
		("h,help", "Produce help message.")
		("i,input-paths-file", "The XML file that is part of a dump of Wikipedia.", cxxopts::value<string>())
		("o,output-sqlite-file", "The folder in which the results should be stored. WARNING: always has to end with / or \\.", cxxopts::value<string>())

		// misc
		("t,show-timings", "Show the timings for the different parsing steps.")
		;

	options.parse(argc, argv);

	// display help if --help was specified
	if (options.count("help")) {
		cout << options.help() << endl;
		return 0;
	}

	if(!options.count("input-paths-file") || !options.count("output-sqlite-file"))
	{
		std::cerr << "Please specify the parameters --input-xml-file and --output-sqlite-file." << std::endl;
		return 1;
	}

	const std::string input_paths_filepath = options["input-paths-file"].as<std::string>();
	const std::string output_sqlite_filepath = options["output-sqlite-file"].as<std::string>();

	std::vector<std::string> paths = read_lines_from_file(std::cin);

	try {

		// reset sqlite db
		clear_sqlite_db(output_sqlite_filepath);

		// init xerces
		xercesc::XMLPlatformUtils::Initialize();

		WikiXmlDumpXerces::WikiDumpHandlerProperties parser_properties;
		parser_properties.TitleFilter = [](const std::string& title) {
			return title.substr(0,5) == "Talk:";
		};

		XmlToSqliteHandler handler(output_sqlite_filepath);
		handler.status_callbacks = [](const std::string& msg) { std::cout << msg << std::endl; };
		WikiXmlDumpXerces::SingleCoreParser parser(handler, parser_properties);
		parser.Run(paths.begin(),paths.end());

		std::cout << "Finished parsing all talk pages in XML file" << std::endl;

		xercesc::XMLPlatformUtils::Terminate();
	}
	catch(const std::exception& exception) {
		std::cerr << "--------------------------------------------------" << std::endl;
		std::cerr << "FATAL ERROR: The application terminated with an exception:" << std::endl;
		std::cerr << exception.what() << std::endl;
		std::cerr << "--------------------------------------------------" << std::endl;
	}

	timings.stopTiming("global");

	if(options.count("show-timings") && options["show-timings"].as<bool>())
	{
		std::cout << std::endl << "--- Timings ---" << std::endl;
		timings.printTimings(std::cout);
		std::cout << std::endl;
	}

	return 0;
}
