#include "getIdsFromDb.h"
#include <sstream>

namespace {
	int get_ids_callback(void* userdata, int /*col_count*/, char** row_data, char** /*col_names*/)
	{
		std::vector<std::size_t>* p_article_ids = static_cast<std::vector<std::size_t>*>(userdata);

		char** str_end = nullptr;
		p_article_ids->push_back(std::strtol(row_data[0], str_end, 10));

		return 0;
	}

	std::vector<std::size_t> get_ids_from_db(sqlite3* sqlite_db, const std::vector<std::string>& elements, const std::string table, const std::string column)
	{
		std::stringstream ss;
		ss << "SELECT id FROM " << table << " WHERE " << column << " IN (";
		bool first = true;
		for(const auto& e : elements) 
		{
			ss << ((first) ? "" : ",") << "'" << e << "'";
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
}

std::vector<std::size_t> get_user_ids_from_db(sqlite3* sqlite_db, const std::vector<std::string>& usernames)
{
	return get_ids_from_db(sqlite_db, usernames, "user", "name");
}

std::vector<std::size_t> get_article_ids_from_db(sqlite3* sqlite_db, const std::vector<std::string>& titles)
{
	return get_ids_from_db(sqlite_db, titles, "article", "title");
}
