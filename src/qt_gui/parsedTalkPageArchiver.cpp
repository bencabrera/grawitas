#include "parsedTalkPageArchiver.h"

#include "parsing/coreTalkPageParsing.h"

ParsedTalkPageArchiver::ParsedTalkPageArchiver()
{

}


void ParsedTalkPageArchiver::parse_talk_page(std::string normalized_title, std::string long_title, std::string content)
{
    auto parsed = Grawitas::parse_talk_page(content);

    auto it = _parsed_talk_pages.find(normalized_title);
    if(it != _parsed_talk_pages.end())
    {
        auto& list = it->second;
        list.splice(list.end(), parsed);
    }
    else
        _parsed_talk_pages.insert({ normalized_title, parsed });

    for(auto& f : status_callbacks)
        f(std::string("Parsing page '") + long_title + std::string("'"));
}

void ParsedTalkPageArchiver::finish_and_export_talk_page(std::string normalized_title)
{
    auto it = _parsed_talk_pages.find(normalized_title);
    if(it == _parsed_talk_pages.end())
        return;

    auto& parsed = it->second;
    std::size_t cur_id = 1;
    for (auto& sec : parsed) {
        calculate_ids(sec.second, cur_id);
    }

    write_finished_talk_page(normalized_title, parsed);
    _parsed_talk_pages.erase(it);

    for(auto& f : status_callbacks)
        f(std::string("Finished parsing all parts of '") + normalized_title + "'");
}
