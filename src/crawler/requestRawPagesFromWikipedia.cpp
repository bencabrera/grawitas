#include "requestRawPagesFromWikipedia.h"

#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <sstream>

std::vector<TalkPageResult> request_raw_pages_from_wikipedia(std::vector<std::string> page_titles)
{
	static const std::string WIKIPEDIA_API_URL = "https://en.wikipedia.org/w/api.php";

	// build up url to use for request
	auto url = QUrl(QString::fromStdString(WIKIPEDIA_API_URL));

	QUrlQuery query;
	query.addQueryItem("action","query");
	query.addQueryItem("prop","revisions");
	query.addQueryItem("rvprop","content");
	query.addQueryItem("format","json");

	std::stringstream parameters;
	bool first = true;
	for(auto title : page_titles)
	{
		if(first)
			first = false;
		else
			parameters << "|";

		parameters << QUrl::toPercentEncoding(QString::fromStdString(title)).toStdString();
	}
	query.addQueryItem("titles",QString::fromStdString(parameters.str()));
	url.setQuery(query);

	// create custom temporary event loop on stack
	QEventLoop eventLoop;

	// "quit()" the event-loop, when the network request "finished()"
	QNetworkAccessManager mgr;
	std::vector<TalkPageResult> rtn;
	QObject::connect(&mgr, &QNetworkAccessManager::finished, [&rtn](QNetworkReply* reply) { 

		if (reply->error() == QNetworkReply::NoError) {
			// success
			auto doc = QJsonDocument::fromJson(reply->readAll());
			auto obj = doc.object();
			auto pages = obj.value("query").toObject().value("pages").toObject();

			for(auto it = pages.begin(); it != pages.end(); it++)
			{
				auto cur_obj = it->toObject();

				TalkPageResult tmp;
				tmp.missing = cur_obj.contains("missing");
				tmp.title = cur_obj.value("title").toString().toStdString();
				if(!tmp.missing)
					tmp.content = cur_obj.value("revisions").toArray().at(0).toObject().value("*").toString().toStdString();

				rtn.push_back(tmp);
			}
		}
		else
		{
			//failure
			qDebug() << "Failure" << reply->errorString();
		}
		reply->deleteLater(); 
	});

	QObject::connect(&mgr, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);

	// the HTTP request
	mgr.get(QNetworkRequest(url));
	eventLoop.exec(); // blocks stack until "finished()" has been called

	return rtn;
}
