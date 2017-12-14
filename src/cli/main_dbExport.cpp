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

#include <cstdlib>
#include <sqlite3.h>

using namespace Grawitas;

namespace {
	int get_comments_callback(void* userdata, int /*col_count*/, char** row_data, char** /*col_names*/)
	{
		std::vector<Grawitas::Comment>* p_comments = static_cast<std::vector<Grawitas::Comment>*>(userdata);

		//id, parent_id, user_id, article_id, date, text
		
		char** str_end = nullptr;

		Grawitas::Comment comment;
		comment.Id = std::strtol(row_data[0], str_end, 10);
		comment.ParentId = std::strtol(row_data[1], str_end, 10);
		comment.User = row_data[2];
		comment.Article = row_data[3];
		comment.Text = row_data[5];
			
		p_comments->push_back(comment);

		return 0;
	};

	int get_ids_callback(void* userdata, int /*col_count*/, char** row_data, char** /*col_names*/)
	{
		std::vector<std::size_t>* p_article_ids = static_cast<std::vector<std::size_t>*>(userdata);

		char** str_end = nullptr;
		p_article_ids->push_back(std::strtol(row_data[0], str_end, 10));

		return 0;
	};

	std::vector<std::size_t> retrieve_article_ids(sqlite3* sqlite_db, const std::vector<std::string>& titles)
	{
		std::stringstream ss;
		ss << "SELECT id FROM article WHERE title IN (";
		bool first = true;
		for(const auto& title : titles) 
		{
			ss << ((first) ? "" : ",") << "'" << title << "'";
			first = false;
		}
		ss << ");";

		auto query = ss.str();

		// Execute SQL statement
		std::vector<std::size_t> article_ids;
		char* zErrMsg;
		auto rc = sqlite3_exec(sqlite_db, query.c_str(), get_ids_callback, &article_ids, &zErrMsg);

		if(rc != SQLITE_OK){
			std::string msg = zErrMsg;
			sqlite3_free(zErrMsg);
			sqlite3_close(sqlite_db);
			throw std::logic_error("SQL error: " + msg);
		} 

		return article_ids;
	}

	std::vector<std::size_t> retrieve_user_ids(sqlite3* sqlite_db, const std::vector<std::string>& usernames)
	{
		std::stringstream ss;
		ss << "SELECT id FROM user WHERE name IN (";
		bool first = true;
		for(const auto& username : usernames) 
		{
			ss << ((first) ? "" : ",") << "'" << username << "'";
			first = false;
		}
		ss << ");";

		auto query = ss.str();

		// Execute SQL statement
		std::vector<std::size_t> user_ids;
		char* zErrMsg;
		auto rc = sqlite3_exec(sqlite_db, query.c_str(), get_ids_callback, &user_ids, &zErrMsg);

		if(rc != SQLITE_OK){
			std::string msg = zErrMsg;
			sqlite3_free(zErrMsg);
			sqlite3_close(sqlite_db);
			throw std::logic_error("SQL error: " + msg);
		} 

		return user_ids;
	}

	std::vector<Grawitas::Comment> retrieve_comments(sqlite3* sqlite_db, const std::vector<std::size_t>& user_ids, const std::vector<std::size_t>& article_ids)
	{
		std::stringstream ss;
		ss << "SELECT id, parent_id, user_id, article_id, date, text  FROM comment";

		if(user_ids.size() > 0)
		{
	   		ss << " WHERE user_id IN (";
			bool first = true;
			for(const auto& user_id : user_ids) 
			{
				ss << ((first) ? "" : ",") << user_id;
				first = false;
			}
			ss << ")";
		}

		if(user_ids.size() > 0 && article_ids.size() > 0)
	   		ss << " AND article_id IN (";
		if(user_ids.size() == 0 && article_ids.size() > 0)
	   		ss << " WHERE article_id IN (";
		if(article_ids.size() > 0)
		{
			bool first = true;
			for(const auto& article_id : article_ids) 
			{
				ss << ((first) ? "" : ",") << article_id;
				first = false;
			}
			ss << ")";
		}
		ss << ";";

		auto query = ss.str();

		// Execute SQL statement
		std::vector<Grawitas::Comment> comments;
		char* zErrMsg;
		auto rc = sqlite3_exec(sqlite_db, query.c_str(), get_comments_callback, &comments, &zErrMsg);

		if(rc != SQLITE_OK){
			std::string msg = zErrMsg;
			sqlite3_free(zErrMsg);
			sqlite3_close(sqlite_db);
			throw std::logic_error("SQL error: " + msg);
		} 

		return comments;
	}
}

int main(int argc, char** argv) 
{
	StepTimer timings;
	timings.startTiming("global", "Total");

    cxxopts::Options options("grawitas_cli_xml", "Parses talk pages in Wikipedia xml dumps to a sqlite file containing the structured comments.");
	options.add_options()
		("h,help", "Produces this help message.")
		("i,sqlite-file", "", cxxopts::value<std::string>())
		("a,article-filter-file", "", cxxopts::value<std::string>())
		("u,user-filter-file", "", cxxopts::value<std::string>())

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

		article_ids = retrieve_article_ids(sqlite_db, article_titles);
	}

	if(options.count("user-filter-file"))
	{
		const std::string user_filter_file_path = options["user-filter-file"].as<std::string>();

		std::ifstream user_filter_file(user_filter_file_path);
		std::vector<std::string> usernames = read_lines_from_file(user_filter_file);

		user_ids = retrieve_user_ids(sqlite_db, usernames);
	}

	auto comments = retrieve_comments(sqlite_db, user_ids, article_ids);

	std::map<Format, std::string> formats;
	for (auto form_parameter : FormatParameterStrings) {
		if(options.count(form_parameter))
			formats.insert({ parameter_to_format(form_parameter), options[form_parameter].as<std::string>() });
	}
	output_in_formats_to_files(formats, comments, {"id", "parent_id", "user", "date", "section", "article", "text"});


	return 0;
}
