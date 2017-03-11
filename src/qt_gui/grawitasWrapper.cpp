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

void GrawitasWrapper::request()
{
    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("https://en.wikipedia.org/w/api.php?action=query&format=json&prop=revisions&titles=Talk:Photosynthesis&rvprop=content") ) );
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
        //qDebug() << reply->readAll();
        auto doc = QJsonDocument::fromJson(reply->readAll());
        //auto obj = doc.object().value("query").toObject().value("pages").toObject().begin()->toObject().value("revisions").toObject().begin()->toObject().value("content").toString();
        auto obj = doc.object();
        qDebug() << "Hallo";
        for(auto i : (*obj.value("query").toObject().value("pages").toObject().begin()).toObject().value("revisions").toObject().keys())
            qDebug() << i;
        //qDebug() << obj;
        //qDebug() << reply->readAll();
        //qDebug() << "Success" << obj["query.pages.46257.revisions.0.content"].toString();
        delete reply;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }
}
