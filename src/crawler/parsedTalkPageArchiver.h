#ifndef PARSEDTALKPAGEARCHIVER_H
#define PARSEDTALKPAGEARCHIVER_H

#include "../core/models/parsedTalkPage.h"
#include <map>
#include <set>
#include <functional>
#include "../core/output/formats.h"

#include <QObject>

class ParsedTalkPageArchiver : public QObject
{
    Q_OBJECT

public:
    ParsedTalkPageArchiver(const std::set<Grawitas::Format> formats, const std::string output_folder);

protected:
    const std::set<Grawitas::Format> _formats;
    const std::string _output_folder;

    std::map<std::string, Grawitas::ParsedTalkPage> _parsed_talk_pages;
	void write_finished_talk_page(std::string title, const Grawitas::ParsedTalkPage& parsed_talk_page);

public slots:
    void parse_talk_page(std::string normalized_title, std::string long_title, std::string content);
    void finish_and_export_talk_page(std::string normalized_title);

signals:
    void write_status(std::string status_message);
};

#endif // PARSEDTALKPAGEARCHIVER_H
