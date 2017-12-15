#include <iostream>
#include <fstream>

#include "../../libs/cxxopts/include/cxxopts.hpp"
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../misc/stepTimer.h"
#include "../misc/readLinesFromFile.h"
#include "../output/formats.h"
#include "../output/outputHelpers.h"
#include "../talkPageParser/models.h"
#include "../output/outputWrapper.h"
#include "../xmlToSql/getArticlesFromDb.h"
#include "../xmlToSql/getUsersFromDb.h"
#include "../xmlToSql/getFilteredCommentsFromDb.h"
#include "../xmlToSql/getIdsFromDb.h"

#include <cstdlib>
#include <sqlite3.h>

using namespace Grawitas;


int main(int argc, char** argv) 
{
	StepTimer timings;
	timings.startTiming("global", "Total");

	cxxopts::Options options("grawitas_cli_xml", "Parses talk pages in Wikipedia xml dumps to a sqlite file containing the structured comments.");
	options.add_options()
		("h,help", "Produces this help message.")
		("i,sqlite-file", "Sqlite file created by grawitas_cli_xml_to_db.", cxxopts::value<std::string>())
		("a,article-filter-file", "File containing articles for which to extract comments.", cxxopts::value<std::string>())
		("u,user-filter-file", "File containing users for which to extract comments.", cxxopts::value<std::string>())

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

		("t,show-timings", "Flag to show timings.")
		;
	options.positional_help("<input-paths-file> <output-sqlite-file>");

	options.parse_positional(std::vector<std::string>{"input-paths-file","output-sqlite-file"});
	options.parse(argc, argv);

	// display help if --help was specified
	if (options.count("help")) {
		std::cout << options.help() << std::endl;
		return 0;
	}

	if(!options.count("sqlite-file"))
		throw std::invalid_argument("Input sqlite database file not specified.");

	const std::string sqlite_file_path = options["sqlite-file"].as<std::string>();

	// Open database
	sqlite3* sqlite_db;
	auto rc = sqlite3_open(sqlite_file_path.c_str(), &sqlite_db);

	if(rc) 
	{
		std::string msg = sqlite3_errmsg(sqlite_db);
		sqlite3_close(sqlite_db);
		throw std::logic_error("Can't open database: %s\n" + msg);
	}

	std::vector<std::size_t> article_ids, user_ids;
	if(options.count("article-filter-file"))
	{
		const std::string article_filter_file_path = options["article-filter-file"].as<std::string>();

		std::ifstream article_filter_file(article_filter_file_path);
		std::vector<std::string> article_titles = read_lines_from_file(article_filter_file);

		article_ids = get_article_ids_from_db(sqlite_db, article_titles);
	}

	if(options.count("user-filter-file"))
	{
		const std::string user_filter_file_path = options["user-filter-file"].as<std::string>();

		std::ifstream user_filter_file(user_filter_file_path);
		std::vector<std::string> usernames = read_lines_from_file(user_filter_file);

		user_ids = get_user_ids_from_db(sqlite_db, usernames);
	}

	auto users = get_users_from_db(sqlite_db);
	auto articles = get_articles_from_db(sqlite_db);
	auto comments = get_filtered_comments(sqlite_db, user_ids, article_ids, &users, &articles);

	std::map<Format, std::string> formats;
	for (auto form_parameter : FormatParameterStrings) {
		if(options.count(form_parameter))
			formats.insert({ parameter_to_format(form_parameter), options[form_parameter].as<std::string>() });
	}

	output_in_formats_to_files(formats, comments, {"id", "parent_id", "user", "date", "section", "article", "text"});


	return 0;
}
