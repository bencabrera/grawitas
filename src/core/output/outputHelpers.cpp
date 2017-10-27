#include "outputHelpers.h"

std::string normalize_folder_path(std::string str)
{
	if(str.back() == '/')
		return str.substr(0, str.length()-1);
	else
		return str;
}
