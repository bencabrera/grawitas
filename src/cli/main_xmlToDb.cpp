#include <iostream>
#include <fstream>

#include "../../libs/cxxopts/include/cxxopts.hpp"
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../misc/stepTimer.h"
#include "../misc/readLinesFromFile.h"
#include "../output/formats.h"
#include "../output/outputHelpers.h"

#include "../xmlToSql/xmlToSqliteHandler.h"
#include "../../libs/wiki_xml_dump_xerces/src/parsers/singleCoreParser.hpp"
#include "../../libs/wiki_xml_dump_xerces/src/handlers/wikiDumpHandlerProperties.hpp"

using namespace Grawitas;
using namespace std;

namespace {
	void run_sql_query_without_result(sqlite3* sqlite_db, const std::string query)
	{
		// Execute SQL statement
		char* zErrMsg;
		auto rc = sqlite3_exec(sqlite_db, query.c_str(), nullptr, 0, &zErrMsg);

		if(rc != SQLITE_OK){
			std::string msg = zErrMsg;
			sqlite3_free(zErrMsg);
			sqlite3_close(sqlite_db);
			throw std::logic_error("SQL error: " + msg);
		} 
	}

	void reset_db(sqlite3* sqlite_db)
	{
		run_sql_query_without_result(sqlite_db, "DROP TABLE IF EXISTS comment;");
		run_sql_query_without_result(sqlite_db, "DROP TABLE IF EXISTS user;");
		run_sql_query_without_result(sqlite_db, "DROP TABLE IF EXISTS article;");

		run_sql_query_without_result(sqlite_db, "CREATE TABLE comment(id INTEGER PRIMARY KEY, parent_id INTEGER NULL, user_id INTEGER, article_id INTEGER, date TEXT, text TEXT);");
		run_sql_query_without_result(sqlite_db, "CREATE TABLE user(id INTEGER PRIMARY KEY, name TEXT);");
		run_sql_query_without_result(sqlite_db, "CREATE TABLE article(id INTEGER PRIMARY KEY, title TEXT);");
	}
}

int main(int argc, char** argv) 
{
	StepTimer timings;
	timings.startTiming("global", "Total");

    cxxopts::Options options("grawitas_cli_xml", "Parses talk pages in Wikipedia xml dumps to a sqlite file containing the structured comments.");
	options.add_options()
		("h,help", "Produces this help message.")
		("i,input-paths-file", "File of which each line is absolute path to an xml file that is part of a Wikipedia dump.", cxxopts::value<string>())
		("o,output-sqlite-file", "Output sqlite file.", cxxopts::value<string>())
		("t,show-timings", "Flag to show timings.")
		;
	options.positional_help("<input-paths-file> <output-sqlite-file>");

	try {
		options.parse_positional(std::vector<std::string>{"input-paths-file","output-sqlite-file"});
		options.parse(argc, argv);

		// display help if --help was specified
		if (options.count("help")) {
			cout << options.help() << endl;
			return 0;
		}

		if(!options.count("input-paths-file") || !options.count("output-sqlite-file"))
			throw std::invalid_argument("Input paths file or output file not specified.");

		const std::string input_paths_filepath = options["input-paths-file"].as<std::string>();
		const std::string output_sqlite_filepath = options["output-sqlite-file"].as<std::string>();

		std::ifstream input_paths_file(input_paths_filepath);
		std::vector<std::string> paths = read_lines_from_file(input_paths_file);

		// Open database
		sqlite3* sqlite_db;
		auto rc = sqlite3_open(output_sqlite_filepath.c_str(), &sqlite_db);

		if(rc) 
		{
			std::string msg = sqlite3_errmsg(sqlite_db);
			sqlite3_close(sqlite_db);
			throw std::logic_error("Can't open database: %s\n" + msg);
		}

		// reset sqlite db
		reset_db(sqlite_db);

		// init xerces
		xercesc::XMLPlatformUtils::Initialize();

		WikiXmlDumpXerces::WikiDumpHandlerProperties parser_properties;
		parser_properties.TitleFilter = [](const std::string& title) {
			return title.substr(0,5) == "Talk:";
		};

		run_sql_query_without_result(sqlite_db, "BEGIN TRANSACTION;");
		XmlToSqliteHandler handler(sqlite_db);
		handler.status_callbacks = [](const std::string& msg) { std::cout << msg << std::endl; };
		WikiXmlDumpXerces::SingleCoreParser parser(handler, parser_properties);
		parser.Run(paths.begin(),paths.end());
		run_sql_query_without_result(sqlite_db, "END TRANSACTION;");

		// add all users to db
		run_sql_query_without_result(sqlite_db, "BEGIN TRANSACTION;");
		for(const auto& p : handler.user_map)
		{
			run_sql_query_without_result(sqlite_db, "INSERT INTO user(id,name) VALUES(" + std::to_string(p.second) + ", \"" + boost::replace_all_copy(p.first, "\"", "\"\"") + "\");");
		}
		run_sql_query_without_result(sqlite_db, "END TRANSACTION;");

		// add all articles to db
		run_sql_query_without_result(sqlite_db, "BEGIN TRANSACTION;");
		for(const auto& p : handler.article_map)
			run_sql_query_without_result(sqlite_db, "INSERT INTO article(id,title) VALUES(" + std::to_string(p.second) + ", \"" + boost::replace_all_copy(p.first, "\"", "\"\"") + "\");");
		run_sql_query_without_result(sqlite_db, "END TRANSACTION;");

		xercesc::XMLPlatformUtils::Terminate();

		sqlite3_close(sqlite_db);
		timings.stopTiming("global");

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
