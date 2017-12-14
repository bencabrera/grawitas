#pragma once

#include "../talkPageParser/models.h"
#include "formats.h"
#include <map>
#include <set>

namespace Grawitas {

	void output_in_format_to_stream(std::ostream& ostr, Format format, const std::vector<Comment>& parsedTalkPage, bool beautify = false, const std::set<std::string> included_fields = {"id", "parent_id", "user", "date", "level", "text", "section", "article"});
	void output_in_formats_to_files(const std::map<Format, std::string>& formats, const std::vector<Comment>& parsedTalkPage, const std::set<std::string> included_fields = {"id", "parent_id", "user", "date", "level", "text", "section", "article"});

}
