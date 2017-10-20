#pragma once

#include "../output/formats.h"
#include <set>
#include <functional>

namespace Grawitas {
	void xml_dump_parsing(
		std::string input_xml_file, 
		std::string output_folder, 
		const std::set<Format>& formats, 
		std::vector<std::function<void(std::string)>> report_status_callbacks = std::vector<std::function<void(std::string)>>()
	);
}
