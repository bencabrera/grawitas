#pragma once

#include "../talkPageParser/models.h"
#include "formats.h"
#include <map>

namespace Grawitas {

	void output_in_format_to_stream(std::ostream& ostr, Format format, const ParsedTalkPage& parsedTalkPage);
	void output_in_formats_to_files(const std::map<Format, std::string>& formats, const ParsedTalkPage& parsedTalkPage);

}
