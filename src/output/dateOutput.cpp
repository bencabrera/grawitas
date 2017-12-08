#include "dateOutput.h"
#include <sstream>
#include <iomanip>
#include <ctime>

namespace Grawitas {

	std::string to_iso_8601(const std::tm& t)
	{
		std::stringstream ss;
		
		ss << std::setfill('0') << std::setw(4) << (t.tm_year+1900) << "-" << std::setfill('0') << std::setw(2)<< (t.tm_mon+1) << "-" << std::setfill('0') << std::setw(2) << t.tm_mday;
		ss << "T";
		ss << std::setfill('0') << std::setw(2) << t.tm_hour << ":" << std::setfill('0') << std::setw(2) << t.tm_min << ":" << std::setfill('0') << std::setw(2) << t.tm_sec;
		ss << "Z"; // for utc timezone

		return ss.str();
	}

	std::string to_human_readable(const std::tm& t)
	{
		std::stringstream ss;

		char mbstr[100];
		if (!std::strftime(mbstr, sizeof(mbstr), "%c", &t))
			throw std::runtime_error("Date buffer was not large enough.");

		ss << mbstr;

		return ss.str();
	}

}
