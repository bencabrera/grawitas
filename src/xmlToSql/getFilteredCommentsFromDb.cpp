#include "getFilteredCommentsFromDb.h"

#include <tuple>
#include <sstream>
#include <boost/algorithm/string.hpp>

namespace {
	std::tm from_sqlite_date(const std::string str)
	{
		std::vector<std::string> date_time_split;
		boost::split(date_time_split,str,boost::is_any_of(" "));

		std::vector<std::string> date_split;
		boost::split(date_split,date_time_split[0],boost::is_any_of("-"));
		std::vector<std::string> time_split;
		boost::split(time_split,date_time_split[1],boost::is_any_of(":"));

		std::tm rtn{};
		rtn.tm_year = std::stoi(date_split[0]) - 1900;
		rtn.tm_mon = std::stoi(date_split[1]) - 1;
		rtn.tm_mday = std::stoi(date_split[2]);
		rtn.tm_hour = std::stoi(time_split[0]);
		rtn.tm_min = std::stoi(time_split[1]);
		rtn.tm_sec = std::stoi(time_split[2]);

		return rtn;
	}

	int get_comments_callback(void* userdata, int /*col_count*/, char** row_data, char** /*col_names*/)
	{
		auto p_data = static_cast<std::tuple<std::vector<Grawitas::Comment>*, const std::vector<std::string>*, const std::vector<std::string>*>*>(userdata);
		auto* p_comments = std::get<0>(*p_data);
		const std::vector<std::string>* p_users = std::get<1>(*p_data);
		const std::vector<std::string>* p_articles = std::get<2>(*p_data);

		//id, parent_id, user_id, article_id, date, text

		char** str_end = nullptr;

		Grawitas::Comment comment;
		comment.Id = std::strtol(row_data[0], str_end, 10);
		comment.ParentId = std::strtol(row_data[1], str_end, 10);
		comment.User = (*p_users)[std::strtol(row_data[2],str_end,10)];
		comment.Article = (*p_articles)[std::strtol(row_data[3],str_end,10)];
		comment.Date = from_sqlite_date(row_data[4]);
		comment.Section = row_data[5];
		comment.Text = row_data[6];

		p_comments->push_back(comment);

		return 0;
	}
}

std::vector<Grawitas::Comment> get_filtered_comments(sqlite3* sqlite_db, const std::vector<std::size_t>& user_ids, const std::vector<std::size_t>& article_ids, const std::vector<std::string>* p_users, std::vector<std::string>* p_articles)
{
	std::stringstream ss;
	ss << "SELECT id, parent_id, user_id, article_id, date, section, text  FROM comment";

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

	std::tuple<std::vector<Grawitas::Comment>*, const std::vector<std::string>*, const std::vector<std::string>*> data;
	std::get<0>(data) = &comments;
	std::get<1>(data) = p_users;
	std::get<2>(data) = p_articles;

	char* zErrMsg;
	auto rc = sqlite3_exec(sqlite_db, query.c_str(), get_comments_callback, &data, &zErrMsg);

	if(rc != SQLITE_OK){
		std::string msg = zErrMsg;
		sqlite3_free(zErrMsg);
		sqlite3_close(sqlite_db);
		throw std::logic_error("SQL error: " + msg);
	} 

	return comments;
}
