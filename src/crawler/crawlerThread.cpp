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

void CrawlerThread::run()
{
    auto titles = read_titles_from_file(input_file_path);

    TalkPageFetcher fetcher(titles);
    ParsedTalkPageArchiver archiver(formats, output_folder);

	connect(&archiver, SIGNAL(write_status(std::string)), this, SIGNAL(write_status(std::string))); // forward write_status signal from archiver to CrawlerThread

	connect(&fetcher, SIGNAL(start_new_article(std::string,std::string,std::string)), &archiver, SLOT(parse_talk_page(std::string,std::string,std::string)));
	connect(&fetcher, SIGNAL(finish_last_archive(std::string)), &archiver, SLOT(finish_and_export_talk_page(std::string)));
	connect(&fetcher, SIGNAL(write_status(std::string)), this, SIGNAL(write_status(std::string)));
	
    if(options.count(KEEP_TALK_PAGE_FILES) > 0)
    {
		connect(&fetcher, SIGNAL(start_new_article(std::string,std::string,std::string)), this, SLOT(start_raw_talk_page_file(std::string,std::string,std::string)));
		connect(&fetcher, SIGNAL(finish_last_archive(std::string)), this, SLOT(finish_raw_talk_page_file(std::string)));
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
	auto it = raw_talk_page_files.find(normalized_title);
	if(it == raw_talk_page_files.end())
	{
		std::string title_filename = Grawitas::safeEncodeTitleToFilename(normalized_title);
		raw_talk_page_files.insert({ normalized_title, new std::ofstream(output_folder + "/" + title_filename + "_raw.wikimd") });
	}
	*raw_talk_page_files[normalized_title] << content << std::endl;
}

void CrawlerThread::finish_raw_talk_page_file(std::string normalized_title)
{
	delete raw_talk_page_files[normalized_title];
	raw_talk_page_files.erase(normalized_title);
}
