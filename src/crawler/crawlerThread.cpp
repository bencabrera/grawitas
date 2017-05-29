#include "crawlerThread.h"

#include <fstream>
#include <QDebug>

#include "helpers/stepTimer.h"
#include "parsing/coreTalkPageParsing.h"
#include "graphComputation/graphComputationCache.h"
#include "models/parsedTalkPage.h"
#include "output/outputWrapper.h"
#include "parsing/xmlDumpParserWrapper.h"
#include "parsing/xmlDumpParsingHandler.h"
#include "output/formats.h"

#include <boost/algorithm/string/trim.hpp>

#include "talkpageFetcher.h"
#include "parsedTalkPageArchiver.h"

CrawlerThread::CrawlerThread(QString _input_file_path, QString _output_folder, std::set<Grawitas::Format> _formats, std::set<CrawlerOptions> _options)
    :input_file_path(_input_file_path),
      output_folder(_output_folder),
      formats(_formats),
      options(_options)
{}

void CrawlerThread::run()
{
    std::vector<std::string> titles;
    std::ifstream input_file(input_file_path.toStdString());
    std::string line;
    while(std::getline(input_file,line))
    {
        boost::trim(line);
        titles.push_back(line);
    }

    TalkPageFetcher fetcher(titles);
    ParsedTalkPageArchiver archiver;

    archiver.write_finished_talk_page = [this](std::string title, const Grawitas::ParsedTalkPage& parsed_talk_page){
        std::string title_filename = Grawitas::safeEncodeTitleToFilename(title);
        std::map<Grawitas::Format, std::string> formats_with_paths;
        for (auto format : formats)
            formats_with_paths.insert({ format, output_folder.toStdString() + "/" + title_filename + Grawitas::FormatFileExtensions.at(format) });

        Grawitas::output_in_formats_to_files(formats_with_paths, parsed_talk_page);
    };

    fetcher.new_page_callbacks.push_back(std::bind(&ParsedTalkPageArchiver::parse_talk_page, &archiver, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    fetcher.finished_last_archive_callbacks.push_back(std::bind(&ParsedTalkPageArchiver::finish_and_export_talk_page, &archiver, std::placeholders::_1));
    fetcher.status_callbacks.push_back([this](std::string msg){ emit write_status(QString::fromStdString(msg)); });
    archiver.status_callbacks.push_back([this](std::string msg){ emit write_status(QString::fromStdString(msg)); });

    if(options.count(KEEP_TALK_PAGE_FILES) > 0)
    {
        std::map<std::string, std::ofstream*> raw_talk_page_files;

        fetcher.new_page_callbacks.push_back([&raw_talk_page_files, this](std::string normalized_title, std::string, std::string content)
        {
            auto it = raw_talk_page_files.find(normalized_title);
            if(it == raw_talk_page_files.end())
            {
                std::string title_filename = Grawitas::safeEncodeTitleToFilename(normalized_title);
                raw_talk_page_files.insert({ normalized_title, new std::ofstream(output_folder.toStdString() + "/" + title_filename + "_raw.wikimd") });
            }
            *raw_talk_page_files[normalized_title] << content << std::endl;
        });

        fetcher.finished_last_archive_callbacks.push_back([&raw_talk_page_files](std::string normalized_title){
            delete raw_talk_page_files[normalized_title];
            raw_talk_page_files.erase(normalized_title);
        });
    }

    fetcher.run();
}
