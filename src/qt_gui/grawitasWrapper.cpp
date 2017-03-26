#include "grawitasWrapper.h"

#include "helpers/stepTimer.h"
#include "parsing/coreTalkPageParsing.h"
#include "graphComputation/graphComputationCache.h"
#include "models/parsedTalkPage.h"
#include "output/outputWrapper.h"
#include "parsing/xmlDumpParserWrapper.h"
#include "parsing/xmlDumpParsingHandler.h"
#include "output/formats.h"

#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

#include <boost/algorithm/string/trim.hpp>

#include "talkpageFetcher.h"
#include "parsedTalkPageArchiver.h"



GrawitasWrapper::GrawitasWrapper(QObject *parent)
    : QObject(parent),
      _crawler_text_area(nullptr),
      _xml_dump_text_area(nullptr)
{

}

QString GrawitasWrapper::core(QString q_talk_page_syntax, QString format_str)
{
    using namespace Grawitas;

    Format format = readable_to_format(format_str.toStdString());

    std::string talk_page_syntax = q_talk_page_syntax.toStdString();

    ParsedTalkPage parsedTalkPage;

    parsedTalkPage = parse_talk_page(talk_page_syntax);

    std::stringstream ss;

    output_in_format_to_stream(ss,format,parsedTalkPage);

    return QString::fromStdString(ss.str());
}

std::set<Grawitas::Format> GrawitasWrapper::formats_from_variant_list(QVariantList readable_format_strs)
{
    std::set<Grawitas::Format> formats;
    for(auto el : readable_format_strs)
    {
        if(!el.canConvert<QString>())
            return std::set<Grawitas::Format>();
        std::string readable_format_str = el.value<QString>().toStdString();
        formats.insert(Grawitas::readable_to_format(readable_format_str));
    }

    return formats;
}

std::set<GrawitasWrapper::CrawlerOptions> GrawitasWrapper::options_from_variant_list(QVariantList readable_option_strs)
{
    std::set<CrawlerOptions> options;
    for(auto el : readable_option_strs)
    {
        if(!el.canConvert<QString>())
            return std::set<CrawlerOptions>();
        std::string readable_option_str = el.value<QString>().toStdString();
        if(readable_option_str == "Keep crawled talk page files")
            options.insert(KEEP_TALK_PAGE_FILES);
    }

    return options;
}



void GrawitasWrapper::xml_dump_component(QString input_xml_path, QString output_folder, QVariantList readable_format_strs)
{
    auto formats = formats_from_variant_list(readable_format_strs);
    std::string xml_path = input_xml_path.toStdString();
    std::string output_path = output_folder.toStdString();
    Grawitas::xml_dump_parsing(xml_path, output_path, formats);
}


void GrawitasWrapper::write_crawler_status(std::string status_message)
{
    if(_crawler_text_area != nullptr)
    {
        auto current_time = QDateTime::currentDateTime();
        status_message = std::string("[") + current_time.toString(QString("yyyy-MM-dd HH:mm:ss")).toStdString() + std::string("] ") + status_message;
        QMetaObject::invokeMethod(_crawler_text_area,"append",Qt::DirectConnection,Q_ARG(QVariant, QVariant(status_message.c_str())));
    }
}

void GrawitasWrapper::crawler_component(QString input_file_path, QString output_folder, QVariantList readable_format_strs, QVariantList readable_option_strs)
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

    auto formats = formats_from_variant_list(readable_format_strs);
    auto options = options_from_variant_list(readable_option_strs);

    archiver.write_finished_talk_page = [&formats, &output_folder](std::string title, const Grawitas::ParsedTalkPage& parsed_talk_page){
        std::string title_filename = Grawitas::safeEncodeTitleToFilename(title);
        std::map<Grawitas::Format, std::string> formats_with_paths;
        for (auto format : formats)
            formats_with_paths.insert({ format, output_folder.toStdString() + "/" + title_filename + Grawitas::FormatFileExtensions.at(format) });

        Grawitas::output_in_formats_to_files(formats_with_paths, parsed_talk_page);
    };

    fetcher.new_page_callbacks.push_back(std::bind(&ParsedTalkPageArchiver::parse_talk_page, &archiver, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    fetcher.finished_last_archive_callbacks.push_back(std::bind(&ParsedTalkPageArchiver::finish_and_export_talk_page, &archiver, std::placeholders::_1));
    fetcher.status_callbacks.push_back(std::bind(&GrawitasWrapper::write_crawler_status, this, std::placeholders::_1));
    archiver.status_callbacks.push_back(std::bind(&GrawitasWrapper::write_crawler_status, this, std::placeholders::_1));

    if(options.count(KEEP_TALK_PAGE_FILES) > 0)
    {
        std::map<std::string, std::ofstream*> raw_talk_page_files;

        fetcher.new_page_callbacks.push_back([&raw_talk_page_files, &output_folder](std::string normalized_title, std::string, std::string content)
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
