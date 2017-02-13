#pragma once

#include <string>

namespace WikiTalkNet {

	void removeNoWikiStuff(std::string& str);

	void removeHtmlTags(std::string& str);

	void replaceOutdentMarkers(std::string& str);

	void removeWikiMetaCommands(std::string& str);

}
