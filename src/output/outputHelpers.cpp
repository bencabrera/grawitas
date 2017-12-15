#include "outputHelpers.h"
#include <boost/algorithm/string/replace.hpp>

std::string normalize_folder_path(std::string str)
{
	boost::replace_all(str, "\\", "/");
	if(str.back() == '/')
		return str.substr(0, str.length()-1);
	else
		return str;
}
