#pragma once

#include "../output/formats.h"
#include <set>

namespace Grawitas {
	void xml_dump_parsing(std::string input_xml_folder, std::string output_folder, const std::set<Format>& formats);
}
