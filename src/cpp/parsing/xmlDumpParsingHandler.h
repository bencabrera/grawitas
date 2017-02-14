#pragma once

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "../../../libs/wiki_xml_dump_xerces/src/handlers/abstractWikiPageHandler.hpp"

namespace Grawitas {

	class XmlDumpParsingHandler : public WikiXmlDumpXerces::AbstractWikiPageHandler {
		public:

			XmlDumpParsingHandler(const boost::program_options::variables_map& vm);
			void HandleArticle(const WikiXmlDumpXerces::WikiPageData&);

		private:
			std::string safeEncodeTitleToFilename(const std::string& title) const;
			const boost::program_options::variables_map& _programm_options;
			const boost::filesystem::path _output_folder;
	};

}
