#include "grawitasWrapper.h"

#include "helpers/stepTimer.h"
#include "parsing/coreTalkPageParsing.h"
#include "graphComputation/graphComputationCache.h"
#include "models/parsedTalkPage.h"
#include "output/outputWrapper.h"
#include "parsing/xmlDumpParserWrapper.h"

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

#include "talkpageFetcher.h"



GrawitasWrapper::GrawitasWrapper(QObject *parent) : QObject(parent)
{

}

QString GrawitasWrapper::core(QString q_talk_page_syntax, QString format_str)
{
    using namespace Grawitas;

    Format format = readable_to_format(format_str.toStdString());

    std::string talk_page_syntax = q_talk_page_syntax.toStdString();

    ParsedTalkPage parsedTalkPage;

    parsedTalkPage = parseTalkPage(talk_page_syntax);

    // after splitting the talk page into comments compute their relationships based on the extracted indentations
    std::size_t curId = 1;
    for (auto& sec : parsedTalkPage) {
        calculateIds(sec.second, curId);
    }

    std::stringstream ss;

    output_in_format_to_stream(ss,format,parsedTalkPage);

    return QString::fromStdString(ss.str());
}

void GrawitasWrapper::xml_dump_component(QString input_xml_path, QString output_folder, QVariantList readable_format_strs)
{
    std::set<Grawitas::Format> formats;
    for(auto el : readable_format_strs)
    {
        if(!el.canConvert<QString>())
            return;
        std::string readable_format_str = el.value<QString>().toStdString();
        formats.insert(Grawitas::readable_to_format(readable_format_str));
    }

    std::string xml_path = input_xml_path.toStdString();
    std::string output_path = output_folder.toStdString();
    Grawitas::xml_dump_parsing(xml_path, output_path, formats);
}


void GrawitasWrapper::request2()
{
    std::vector<std::string> titles = {
        "Donald Trump"
    };

    TalkPageFetcher fetcher(titles);
    std::ofstream file("/home/ace7k3/Desktop/blubbern.txt");
    fetcher.new_page_callbacks.push_back([&file](std::string title, std::string content){
        file << content;

    });
    fetcher.run();
}


// PROCESS:
// 1. Send request with original title and title + Archive 1 and title + Archive Month
// 2. Check if any of the archive requests was fruitful, if not return talk page
// 3. Otherwise create multiple requests at once with Archive [2-20] ...
// 4. Check how many were succesful: if all then go on

void GrawitasWrapper::request()
{
    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    //QNetworkRequest req( QUrl( QString("https://en.wikipedia.org/w/api.php?action=query&format=json&prop=revisions&titles=Talk:Photosynthesis&rvprop=content") ) );
    QNetworkRequest req( QUrl( QString("https://en.wikipedia.org/w/api.php?action=query&titles=Talk:Donald%20Trump|Talk:Donald%20Trump/Archive%201|Talk:Donald%20Trump/Archive%2060|Talk:Donald%20Trump/Archive%2061&prop=revisions&rvprop=content&format=json") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
        //qDebug() << reply->readAll();
        auto doc = QJsonDocument::fromJson(reply->readAll());
        auto obj = doc.object();

        auto pages = obj.value("query").toObject().value("pages").toObject();
        for(auto i : pages.keys())
            qDebug() << i;

        for(auto it = pages.begin(); it != pages.end(); it++)
        {
            qDebug() << it->toObject().value("title").toString();
            qDebug() << it->toObject().value("revisions").toArray().at(0).toObject().value("*").toString();
        }

        delete reply;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }
}

