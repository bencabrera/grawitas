#include "talkpageFetcher.h"

#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <sstream>

const std::string TalkPageFetcher::WIKIPEDIA_API_URL = "https://en.wikipedia.org/w/api.php";

TalkPageFetcher::TalkPageFetcher(const std::vector<std::string> titles)
{
    for(auto& title : titles)
        _next_titles.push(title);
}

void TalkPageFetcher::run()
{
    while(!_next_titles.empty())
    {
        // get the next n titles
        std::vector<std::string> cur_titles;
        std::size_t n_tmp = std::min(_next_titles.size(),static_cast<std::size_t>(N_PAGES_PER_REQUEST));
        for(std::size_t i = 0; i < n_tmp; i++)
        {
            cur_titles.push_back(_next_titles.top());
            _next_titles.pop();
        }

        auto results = request(cur_titles);

        for(auto& result : results)
        {
            if(!result.missing)
            {
                for(auto& c : new_page_callbacks)
                {
                    auto split = split_title(result.title);
                    c(split.title, result.title, result.content);

                }
            }
        }

        generate_next_titles_to_get(results);
    }
}

std::vector<TalkPageFetcher::TalkPageResult> TalkPageFetcher::request(const std::vector<std::string> titles)
{
    // build up url to use for request
    auto url = QUrl(QString::fromStdString(WIKIPEDIA_API_URL));

    QUrlQuery query;
    query.addQueryItem("action","query");
    query.addQueryItem("prop","revisions");
    query.addQueryItem("rvprop","content");
    query.addQueryItem("format","json");

    std::stringstream parameters;
    bool first = true;
    for(auto title : titles)
    {
        if(first)
            first = false;
        else
            parameters << "|";

        parameters << "Talk:" << QUrl::toPercentEncoding(QString::fromStdString(title)).toStdString();
    }
    query.addQueryItem("titles",QString::fromStdString(parameters.str()));
    url.setQuery(query);

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req(url);
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    std::vector<TalkPageResult> rtn;
    if (reply->error() == QNetworkReply::NoError) {
        //success
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

        delete reply;
    }
    else
    {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }

    return rtn;
}

void TalkPageFetcher::generate_next_titles_to_get(const std::vector<TalkPageFetcher::TalkPageResult>& results)
{
    std::map<std::string, std::pair<bool,TalkPageTitle>> archived_pages;
    for(auto& result : results)
    {
        auto split = split_title(result.title);
        if(!split.is_archive)
            _next_titles.push(split.title + "/Archive 1");
        else
        {
            auto it = archived_pages.find(split.title);
            if(it == archived_pages.end())
                archived_pages.insert({ split.title, { result.missing, split} });
            else
                if(it->second.second.i_archive < split.i_archive)
                it->second = std::pair<bool,TalkPageTitle>{ result.missing, split};
        }
    }

    for(auto& archived_page : archived_pages)
    {
        if(archived_page.second.first)
        {
            for(auto& f : finished_last_archive_callbacks)
                f(archived_page.second.second.title);
        }
        else
        {
            for(std::size_t i = 0; i < N_NEXT_ARCHIVES; i++)
                _next_titles.push(archived_page.second.second.title + "/Archive " + std::to_string(archived_page.second.second.i_archive+i+1));
        }
    }
}

TalkPageFetcher::TalkPageTitle TalkPageFetcher::split_title(std::string title)
{
    TalkPageTitle tmp;

    if(title.substr(0,5) == "Talk:")
        title = title.substr(5);

    auto pos = title.find("/Archive");
    tmp.is_archive = (pos != std::string::npos);

    if(tmp.is_archive)
    {
        tmp.title = title.substr(0,pos);
        std::string archive_str = title.substr(pos+9);
        tmp.i_archive = std::stol(archive_str);
    }
    else
    {
        tmp.title = title;
    }

    return tmp;
}

