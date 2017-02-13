#pragma once 

#include <ctime>
#include <iostream>

namespace Grawitas {

	std::string to_iso_8601(const std::tm& t);
	std::string to_human_readable(const std::tm& t);

}
