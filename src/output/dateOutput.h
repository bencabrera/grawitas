#pragma once 

#include <ctime>
#include <iostream>

#include "../talkPageParser/date.h"

namespace Grawitas {

	std::string to_iso_8601(const Date& t);
	std::string to_human_readable(const Date& t);

}
