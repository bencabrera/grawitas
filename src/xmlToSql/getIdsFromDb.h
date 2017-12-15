#include <vector>
#include <string>
#include <sqlite3.h>

std::vector<std::size_t> get_user_ids_from_db(sqlite3* sqlite_db, const std::vector<std::string>& usernames);
std::vector<std::size_t> get_article_ids_from_db(sqlite3* sqlite_db, const std::vector<std::string>& titles);
