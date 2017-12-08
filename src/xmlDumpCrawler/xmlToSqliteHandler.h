#pragma once

#include <functional>
#include <set>

#include "../../libs/wiki_xml_dump_xerces/src/handlers/abstractWikiPageHandler.hpp"
#include "../output/formats.h"

#include <sqlite3.h> 

namespace Grawitas {

	class XmlToSqliteHandler : public WikiXmlDumpXerces::AbstractWikiPageHandler {
		public:

			XmlToSqliteHandler(const std::string& sqlite_filepath);
			~XmlToSqliteHandler();

			void HandleArticle(const WikiXmlDumpXerces::WikiPageData&);

			std::function<void(const std::string&)> status_callbacks;

		private:
			sqlite3* sqlite_db;
	};

}
