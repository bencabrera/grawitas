#include "xmlToSqliteHandler.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../talkPageParser/parsing.h"
#include "../output/outputWrapper.h"

namespace {
	int callback(void* /*NotUsed*/, int argc, char **argv, char **azColName) {
		int i;
		for(i = 0; i<argc; i++) {
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
	}

	void add_talk_page_to_db(sqlite3* db, const std::string& title, int archive, std::string content) 
	{
		char* zErrMsg = 0;
		int rc;

		boost::replace_all(content, "\"", "\"\"");

		std::string query = "INSERT INTO talk_page(title, archive, content) VALUES(\""+title+"\", "+std::to_string(archive)+", \""+content+"\");"; 

		/* Execute SQL statement */
		rc = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);

		if(rc != SQLITE_OK){
			std::string msg = zErrMsg;
			sqlite3_free(zErrMsg);
			throw std::logic_error("SQL error: %s\n" + msg);
		} 
	}

	struct TalkPageTitle {
		std::string title;
		bool is_archive;
		int i_archive;
	};

	TalkPageTitle parse_page_title(std::string title)
	{
		TalkPageTitle tmp;

		if(title.substr(0,5) == "Talk:")
			title = title.substr(5);

		auto pos = title.find("/Archive");
		tmp.is_archive = (pos != std::string::npos);

		if(tmp.is_archive)
		{
			tmp.title = title.substr(0,pos);
			boost::trim(tmp.title);

			if(pos+9 < title.size())
			{
				std::string archive_str = title.substr(pos+9);
				try {
					tmp.i_archive = std::stol(archive_str);
				}
				catch(const std::exception& e) {
					tmp.i_archive = 1;
				}
			}
			else
				tmp.i_archive = 1;
		}
		else
			tmp.title = title;

		return tmp;
	}
}

namespace Grawitas {


	XmlToSqliteHandler::XmlToSqliteHandler(const std::string& sqlite_filepath)
	{
		char* zErrMsg = 0;
		int rc;

		// Open database
		rc = sqlite3_open(sqlite_filepath.c_str(), &sqlite_db);

		if(rc) 
		{
			std::string msg = sqlite3_errmsg(sqlite_db);
			sqlite3_close(sqlite_db);
			throw std::logic_error("Can't open database: %s\n" + msg);
		}

		std::string query = "CREATE TABLE IF NOT EXISTS talk_page(id ROWID, archive INTEGER, title VARCHAR(512), content TEXT);";

		// Execute SQL statement
		rc = sqlite3_exec(sqlite_db, query.c_str(), callback, 0, &zErrMsg);

		if(rc != SQLITE_OK){
			std::string msg = zErrMsg;
			sqlite3_free(zErrMsg);
			sqlite3_close(sqlite_db);
			throw std::logic_error("SQL error: %s\n" + msg);
		} 

		sqlite3_exec(sqlite_db, "BEGIN TRANSACTION", callback, 0, &zErrMsg);
	}

	XmlToSqliteHandler::~XmlToSqliteHandler()
	{
		char* zErrMsg = 0;
		sqlite3_exec(sqlite_db, "END TRANSACTION", callback, 0, &zErrMsg);
		sqlite3_close(sqlite_db);
	}

	void XmlToSqliteHandler::HandleArticle(const WikiXmlDumpXerces::WikiPageData& data)
	{
		std::string title = data.MetaData.at("title");
		std::cout << title << std::endl;
		auto split = parse_page_title(title);
		auto i_archive = split.is_archive ? split.i_archive : 0;
		add_talk_page_to_db(sqlite_db, split.title, i_archive, data.Content);
	}

}
