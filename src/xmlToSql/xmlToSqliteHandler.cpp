#include "xmlToSqliteHandler.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../talkPageParser/parsing.h"
#include "../output/outputWrapper.h"

namespace {

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

	void run_sql_query_without_result(sqlite3* sqlite_db, std::string query)
	{
		// Execute SQL statement
		char* zErrMsg = nullptr;
		int rc = sqlite3_exec(sqlite_db, query.c_str(), nullptr, 0, &zErrMsg);
		if(rc != SQLITE_OK){
			if(zErrMsg != nullptr)
			{
				std::string msg = zErrMsg;
				sqlite3_free(zErrMsg);
				throw std::logic_error("SQL error: " + msg);
			}
			else
				throw std::logic_error("SQL error without error message.");
		} 
	}

	std::string to_sqlite_date(const std::tm& t)
	{
		std::stringstream ss;

		ss << std::setfill('0') << std::setw(4) << (t.tm_year+1900) << "-" << std::setfill('0') << std::setw(2)<< (t.tm_mon+1) << "-" << std::setfill('0') << std::setw(2) << t.tm_mday;
		ss << " ";
		ss << std::setfill('0') << std::setw(2) << t.tm_hour << ":" << std::setfill('0') << std::setw(2) << t.tm_min << ":" << std::setfill('0') << std::setw(2) << t.tm_sec;

		return ss.str();
	}
}

namespace Grawitas {

	XmlToSqliteHandler::XmlToSqliteHandler(sqlite3* db)
	:sqlite_db(db), i_user(1), i_article(1), i_comment(1)
	{}

	void XmlToSqliteHandler::HandleArticle(const WikiXmlDumpXerces::WikiPageData& data)
	{
		std::string title = data.MetaData.at("title");
		std::cout << title << " " << i_comment << std::endl;
		auto split = parse_page_title(title);
		auto comments = parse_talk_page(data.Content);
		std::size_t n_article_comments=0;
		for(const auto& c : comments) 
		{
			insert_comment(c, split.title);
			n_article_comments++;
		}
		i_comment += n_article_comments;
	}

	void XmlToSqliteHandler::insert_comment(const Grawitas::Comment& comment, const std::string& article_title)
	{
		// get or add user to map
		auto user_it = user_map.find(comment.User);
		if(user_it == user_map.end())
			user_it = user_map.insert({ comment.User, i_user++ }).first;

		// get or add article to map
		auto article_it = article_map.find(article_title);
		if(article_it == article_map.end())
			article_it = article_map.insert({ article_title, i_article++ }).first;

		auto text = comment.Text;
		boost::replace_all(text, "\"", "\"\"");
		auto section = comment.Section;
		boost::replace_all(section, "\"", "\"\"");

		std::stringstream ss;
		ss << "INSERT INTO comment(id, parent_id, user_id, article_id, date, section, text) VALUES(";
		ss << i_comment+comment.Id << ",";
		ss << ((i_comment == 0) ? 0 :  i_comment+comment.ParentId) << ",";
		ss << user_it->second << ",";
		ss << article_it->second << ",";
		ss << "\"" << to_sqlite_date(comment.Date) << "\",";
		ss << "\"" << section << "\",";
		ss << "\"" << text << "\");";

		std::string query = ss.str();
		run_sql_query_without_result(sqlite_db, query);

		if(i_comment % 10000 == 0)
		{
			run_sql_query_without_result(sqlite_db, "END TRANSACTION");
			run_sql_query_without_result(sqlite_db, "BEGIN TRANSACTION");
		}
	}
}
