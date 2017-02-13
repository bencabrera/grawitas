#include "cleaningSyntax.h"

#include <iomanip>
#include <regex>

namespace WikiTalkNet {

	void removeNoWikiStuff(std::string& str)
	{
		const std::regex newlineRegex("<nowiki>[^<]*</nowiki>");
		str = std::regex_replace(str, newlineRegex, "");
	}

	void removeHtmlTags(std::string& str)
	{
		const std::regex htmlOpeningRegex("<\\w+[^>]+>");
		const std::regex htmlClosingRegex("</\\w+[\\w\\s\\d=\"#:;-]+>");
		const std::regex htmlEscapesRegex("&\\w{1,4};");
		str = std::regex_replace(str, htmlOpeningRegex, "");
		str = std::regex_replace(str, htmlClosingRegex, "");
		str = std::regex_replace(str, htmlEscapesRegex, "");
	}

	void replaceOutdentMarkers(std::string& str)
	{
		const std::regex oi1("\\{\\{outdent\\}\\}");
		const std::regex oi2("\\{\\{od\\}\\}");
		const std::regex oi3("\\{\\{Outdent\\}\\}");
		str = std::regex_replace(str, oi1, "/qqqOUTDENTqqq/");
		str = std::regex_replace(str, oi2, "/qqqOUTDENTqqq/");
		str = std::regex_replace(str, oi3, "/qqqOUTDENTqqq/");

		const std::regex oi4("\\{\\{outdent\\|([^\\}]+)\\}\\}");
		const std::regex oi5("\\{\\{od\\|([^\\}]+)\\}\\}");
		const std::regex oi6("\\{\\{Outdent\\|([^\\}]+)\\}\\}");
		str = std::regex_replace(str, oi4, "/qqqOUTDENTqqq$1/");
		str = std::regex_replace(str, oi5, "/qqqOUTDENTqqq$1/");
		str = std::regex_replace(str, oi6, "/qqqOUTDENTqqq$1/");
	}

	void removeWikiMetaCommands(std::string& str)
	{
		const std::regex wikiMetaCommandsRegex("\\{\\{[^\\}]+\\}\\}");
		const std::regex wikiMetaCommandsRegex2("\\{\\{");
		const std::regex wikiMetaCommandsRegex3("\\}\\}");
		const std::regex wikiMetaCommandsRegex4("'''"); //wikiSyntax for strong
		const std::regex wikiMetaCommandsRegex5("''"); //wikiSyntax for italic
		const std::regex wikiMetaCommentsRegex("<!--[^-]*-->");
		str = std::regex_replace(str, wikiMetaCommandsRegex, "");
		str = std::regex_replace(str, wikiMetaCommandsRegex2, "");
		str = std::regex_replace(str, wikiMetaCommandsRegex3, "");
		str = std::regex_replace(str, wikiMetaCommandsRegex4, "");
		str = std::regex_replace(str, wikiMetaCommandsRegex5, "");
		str = std::regex_replace(str, wikiMetaCommentsRegex, "");
	}
}
