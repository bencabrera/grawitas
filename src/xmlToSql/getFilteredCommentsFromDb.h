#include <vector>
#include <string>
#include <sqlite3.h>
#include "../talkPageParser/models.h"

std::vector<Grawitas::Comment> get_filtered_comments(sqlite3* sqlite_db, const std::vector<std::size_t>& user_ids, const std::vector<std::size_t>& article_ids, const std::vector<std::string>* p_users, std::vector<std::string>* p_articles);
