#include "../talkPageParser/models.h"
#include <vector>
#include <sqlite3.h>

std::vector<std::string> get_articles_from_db(sqlite3* sqlite_db);
