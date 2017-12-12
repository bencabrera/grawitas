#pragma once

#include <functional>
#include <set>

#include "../../libs/wiki_xml_dump_xerces/src/handlers/abstractWikiPageHandler.hpp"
#include "../output/formats.h"

#include <sqlite3.h> 
#include "../talkPageParser/models.h"

namespace Grawitas {

	class XmlToSqliteHandler : public WikiXmlDumpXerces::AbstractWikiPageHandler {
		public:

			XmlToSqliteHandler(sqlite3* db);

			void HandleArticle(const WikiXmlDumpXerces::WikiPageData&);

			std::function<void(const std::string&)> status_callbacks;

			void insert_comment(const Grawitas::Comment& comment, const std::string& article_title);

			std::map<std::string, std::size_t> article_map;
			std::map<std::string, std::size_t> user_map;

		private:
			sqlite3* sqlite_db;

			std::size_t i_user;
			std::size_t i_article;
			std::size_t i_comment;
	};

}
