#include "parsedTalkPageArchiver.h"

#include "parsing/coreTalkPageParsing.h"
#include "output/outputWrapper.h"

ParsedTalkPageArchiver::ParsedTalkPageArchiver(const std::set<Grawitas::Format> formats, const std::string output_folder)
	:_formats(formats),
	_output_folder(output_folder)
{}

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

    emit write_status(std::string("Parsing page '") + long_title + std::string("'"));
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

    emit write_status(std::string("Finished parsing all parts of '") + normalized_title + "'");
}

void ParsedTalkPageArchiver::write_finished_talk_page(std::string title, const Grawitas::ParsedTalkPage& parsed_talk_page)
{
	std::string title_filename = Grawitas::safeEncodeTitleToFilename(title);
	std::map<Grawitas::Format, std::string> formats_with_paths;
	for (const auto& format : _formats)
		formats_with_paths.insert({ format, _output_folder + "/" + title_filename + Grawitas::FormatFileExtensions.at(format) });

	Grawitas::output_in_formats_to_files(formats_with_paths, parsed_talk_page);
}
