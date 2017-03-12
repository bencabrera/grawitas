#ifndef PARSEDTALKPAGEARCHIVER_H
#define PARSEDTALKPAGEARCHIVER_H

#include "models/parsedTalkPage.h"
#include <map>
#include <functional>

class ParsedTalkPageArchiver
{
public:
    ParsedTalkPageArchiver(const std::string output_folder);

    void parse_talk_page(std::string title, std::string content);
    void finish_and_export_talk_page(std::string title);

    std::function<void(std::string, const Grawitas::ParsedTalkPage&)> write_finished_talk_page;

private:
    const std::string _output_folder;

    std::map<std::string, Grawitas::ParsedTalkPage> _parsed_talk_pages;
};

#endif // PARSEDTALKPAGEARCHIVER_H
