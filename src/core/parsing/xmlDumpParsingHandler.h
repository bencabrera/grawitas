#pragma once

#include <functional>
#include <set>
#include <boost/filesystem.hpp>

#include "../../../libs/wiki_xml_dump_xerces/src/handlers/abstractWikiPageHandler.hpp"
#include "../output/formats.h"

namespace Grawitas {

	class XmlDumpParsingHandler : public WikiXmlDumpXerces::AbstractWikiPageHandler {
		public:

			XmlDumpParsingHandler(const std::set<Format>& formats, std::string output_folder);
			void HandleArticle(const WikiXmlDumpXerces::WikiPageData&);

			std::vector<std::function<void(std::string)>> report_status_callbacks;

		private:
			const std::set<Format>& _formats;
			const boost::filesystem::path _output_folder;
	};

}
