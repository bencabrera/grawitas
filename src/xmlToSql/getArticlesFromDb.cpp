#include "getUsersFromDb.h"

#include <stdexcept>

namespace {
	int get_articles_callback(void* userdata, int /*col_count*/, char** row_data, char** /*col_names*/)
	{
		std::vector<std::string>* p_articles = static_cast<std::vector<std::string>*>(userdata);

		char** str_end = nullptr;

		std::size_t id = std::strtol(row_data[0], str_end, 10);
		if(id != p_articles->size())
			throw std::runtime_error("Id did not match vector size");

		p_articles->push_back(row_data[1]);

		return 0;
	};
}

std::vector<std::string> get_articles_from_db(sqlite3* sqlite_db)
{
	std::string query = "SELECT id, title FROM article ORDER BY id ASC;";

	// Execute SQL statement
	std::vector<std::string> articles;
	articles.push_back("DUMMY");
	char* zErrMsg;
	auto rc = sqlite3_exec(sqlite_db, query.c_str(), get_articles_callback, &articles, &zErrMsg);

	if(rc != SQLITE_OK){
		std::string msg = zErrMsg;
		sqlite3_free(zErrMsg);
		sqlite3_close(sqlite_db);
		throw std::logic_error("SQL error: " + msg);
	} 

	return articles;
}
