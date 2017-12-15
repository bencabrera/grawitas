#include <vector>
#include <string>
#include <sqlite3.h>

std::vector<std::string> get_users_from_db(sqlite3* sqlite_db);
