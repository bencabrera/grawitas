#include "grawitasWrapper.h"

#include "../../core/helpers/stepTimer.h"
#include "../../core/parsing/coreTalkPageParsing.h"
#include "../../core/graphComputation/graphComputationCache.h"
#include "../../core/models/parsedTalkPage.h"
#include "../../core/output/outputWrapper.h"
#include "../../core/parsing/xmlDumpParserWrapper.h"
#include "../../core/parsing/xmlDumpParsingHandler.h"
#include "../../core/output/formats.h"

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

#include "../../crawler/talkpageFetcher.h"
#include "../../crawler/parsedTalkPageArchiver.h"



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

std::set<CrawlerOptions> GrawitasWrapper::options_from_variant_list(QVariantList readable_option_strs)
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

    DumpParserThread* thread = new DumpParserThread(input_xml_path, output_folder, formats);
    connect(thread,SIGNAL(write_status(QString)),this,SLOT(write_xml_dump_status(QString)));
    connect(thread,SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void GrawitasWrapper::tab_view_changed()
{
    if(_crawler_text_area == nullptr)
        _crawler_text_area = root_objects.front()->findChild<QObject*>("crawler_status_text_area");
    if(_xml_dump_text_area == nullptr)
        _xml_dump_text_area = root_objects.front()->findChild<QObject*>("xml_status_text_area");
}

void GrawitasWrapper::write_crawler_status(std::string std_status_message)
{
	QString status_message = QString::fromStdString(std_status_message);
    if(_crawler_text_area != nullptr)
    {
        auto current_time = QDateTime::currentDateTime();
        status_message = QString("[") + current_time.toString(QString("yyyy-MM-dd HH:mm:ss")) + QString("] ") + status_message;
        QMetaObject::invokeMethod(_crawler_text_area,"append",Qt::DirectConnection,Q_ARG(QVariant, QVariant(status_message)));
    }
}

void GrawitasWrapper::write_xml_dump_status(QString status_message)
{
    if(_xml_dump_text_area != nullptr)
    {
        auto current_time = QDateTime::currentDateTime();
        status_message = QString("[") + current_time.toString(QString("yyyy-MM-dd HH:mm:ss")) + QString("] ") + status_message;
        QMetaObject::invokeMethod(_xml_dump_text_area,"append",Qt::DirectConnection,Q_ARG(QVariant, QVariant(status_message)));
    }
}

void GrawitasWrapper::crawler_component(QString input_file_path, QString output_folder, QVariantList readable_format_strs, QVariantList readable_option_strs)
{
    auto formats = formats_from_variant_list(readable_format_strs);
    auto options = options_from_variant_list(readable_option_strs);

    CrawlerThread* thread = new CrawlerThread(input_file_path, output_folder, formats, options);
    connect(thread,SIGNAL(write_status(std::string)),this,SLOT(write_crawler_status(std::string)));
    connect(thread,SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
