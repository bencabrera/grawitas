#include "crawlerThread.h"

#include <fstream>
#include <QDebug>

#include "../core/helpers/stepTimer.h"
#include "../core/parsing/coreTalkPageParsing.h"
#include "../core/graphComputation/graphComputationCache.h"
#include "../core/models/parsedTalkPage.h"
#include "../core/parsing/xmlDumpParserWrapper.h"
#include "../core/parsing/xmlDumpParsingHandler.h"
#include "../core/output/formats.h"
#include "../core/output/outputHelpers.h"

#include <boost/algorithm/string/trim.hpp>

#include "talkpageFetcher.h"
#include "parsedTalkPageArchiver.h"

CrawlerThread::CrawlerThread(QString _input_file_path, QString _output_folder, std::set<Grawitas::Format> _formats, std::set<CrawlerOptions> _options)
    :input_file_path(_input_file_path.toStdString()),
      output_folder(normalize_folder_path(_output_folder.toStdString())),
      formats(_formats),
      options(_options)
{}

namespace {
	std::vector<std::string> read_titles_from_file(std::string file_path)
	{
		std::vector<std::string> titles;
		std::ifstream input_file(file_path);
		std::string line;
		while(std::getline(input_file,line))
		{
			boost::trim(line);
			titles.push_back(line);
		}

		return titles;
	}
}

void CrawlerThread::run()
{
    auto titles = read_titles_from_file(input_file_path);

    TalkPageFetcher fetcher(titles);
    ParsedTalkPageArchiver archiver(formats, output_folder);

	connect(&archiver, &ParsedTalkPageArchiver::write_status, this, &CrawlerThread::write_status); // forward write_status signal from archiver to CrawlerThread

	connect(&fetcher, &TalkPageFetcher::start_new_article, &archiver, &ParsedTalkPageArchiver::parse_talk_page);
	connect(&fetcher, &TalkPageFetcher::finish_last_archive, &archiver, &ParsedTalkPageArchiver::finish_and_export_talk_page);
	connect(&fetcher, &TalkPageFetcher::write_status, this, &CrawlerThread::write_status);
	
    if(options.count(KEEP_TALK_PAGE_FILES) > 0)
    {
		connect(&fetcher, &TalkPageFetcher::start_new_article, this, &CrawlerThread::start_raw_talk_page_file);
    }

	try {
    	fetcher.run();
		write_status("Finished crawling and parsing provided articles");
	}
	catch(const std::exception& exception) {
		write_status("--------------------------------------------------");
		write_status("FATAL ERROR: The application terminated with an exception:");
		write_status(exception.what());
		write_status("--------------------------------------------------\n");
	}
}

void CrawlerThread::start_raw_talk_page_file(std::string normalized_title, std::string, std::string content)
{
	std::string title_filename = Grawitas::safeEncodeTitleToFilename(normalized_title);
	std::ofstream raw_file(output_folder + "/" + title_filename + "_raw.wikimd", std::ios::app);
	raw_file << content << std::endl;
}
