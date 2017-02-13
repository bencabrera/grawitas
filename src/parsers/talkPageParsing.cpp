#include <iostream>
#include <list>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include <string>
#include <iomanip>
#include <regex>
#include <stack>

#include "talkPageParsing.h"
#include "regexCombinatoricalParser.h"
#include "../models/comment.h"
#include "commentParser.h"

namespace WikiTalkNet {

	// -------------------------- steps of parsing talk page ---------------------------

	void splitByHeader(std::list<ParagraphWithHeader>& input, unsigned numberOfEqualitySigns)
	{
		std::list<std::pair<std::string, std::string>> rtn;

		std::string equalitySigns = "";
		for(unsigned i = 0; i < numberOfEqualitySigns; i++)
			equalitySigns += "=";

		std::regex headerStrExp("\\n\\s*" + equalitySigns + "(?=[\\w\\s])([^=]+)" + equalitySigns + "(?!=)");
		//std::regex contentAfterHeaderExp("==[\\s]+\\n([^==]+)");

		for(auto listIt = input.begin(); listIt != input.end(); ++listIt)
		{
			auto item = *listIt;
			HeaderDescriptor prevLevelHeader = item.first;
			std::string curContent = item.second;

			auto headerStr_begin = std::sregex_iterator(curContent.begin(), curContent.end(), headerStrExp);
			auto headerStr_end = std::sregex_iterator();

			if(headerStr_begin != headerStr_end)
			{
				std::size_t prevPosition = 0;
				std::size_t prevLength = 0;
				std::string prevHeader = "";
				std::list<ParagraphWithHeader> tmp;
				for (std::sregex_iterator i = headerStr_begin; i != headerStr_end; ++i) {
					std::smatch match = *i;
					auto position = match.position();
					auto length = match.length();

					std::string newContent = curContent.substr(prevPosition+prevLength, position - prevPosition - prevLength);
					HeaderDescriptor newHeader = prevLevelHeader;
					newHeader.push_back(prevHeader);
					tmp.push_back({ newHeader, newContent });

					prevPosition = (size_t) position;
					prevLength = (size_t) length;
					prevHeader = match[1].str();
				}
				std::string newContent = curContent.substr(prevPosition+prevLength, curContent.size() - prevPosition - prevLength);
				HeaderDescriptor newHeader = prevLevelHeader;
				newHeader.push_back(prevHeader);
				tmp.push_back({ newHeader, newContent });

				input.insert(listIt, tmp.begin(), tmp.end());
				input.erase(listIt--);
			}
		}
	}


	std::list<std::string> splitIntoComments(const std::string& str)
	{
		std::list<std::string> rtn;
		rtn.push_back(str);
		splitEachByRegex(rtn, std::regex("\\n:"), true);
		trimEach(rtn, "\n ");

		RegexCombinatoricalParser combiParser({ STRONG_USER_REG_EXPS, FILLER_REG_EXP, DATE_REG_EXPS, { "\\n" } });
		for(auto regexStr : combiParser.getAllRegexStrings())
		{
			splitEachByRegex(rtn, std::regex(regexStr), false);
			trimEach(rtn, "\n ");
		}

		//splitEachByRegex(rtn, std::regex("\\[\\[\\S+\\]\\] [0-9]{2}:[0-9]{2}, [0-9]{2} \\w{3} [0-9]{4} \\(UTC\\)\\n"), false);
		//trimEach(rtn, "\n ");
		splitEachByRegex(rtn, std::regex("\\(anon post\\)\\n", std::regex::ECMAScript | std::regex::icase), false);
		trimEach(rtn, "\n ");
		splitEachByRegex(rtn, std::regex("\\[anon post\\]\\n", std::regex::ECMAScript | std::regex::icase), false);
		trimEach(rtn, "\n ");
		splitEachByRegex(rtn, std::regex("\\(Anonymous post\\)\\n", std::regex::ECMAScript | std::regex::icase), false);
		trimEach(rtn, "\n ");
		splitEachByRegex(rtn, std::regex("\\[Anonymous post\\]\\n", std::regex::ECMAScript | std::regex::icase), false);
		trimEach(rtn, "\n ");
		splitEachByRegex(rtn, std::regex("\\(Anonymous\\)\\n", std::regex::ECMAScript | std::regex::icase), false);
		trimEach(rtn, "\n ");
		splitEachByRegex(rtn, std::regex("\\[Anonymous\\]\\n", std::regex::ECMAScript | std::regex::icase), false);
		trimEach(rtn, "\n ");

		if(rtn.size() < 3){
			// count how often * appears and how often :
			unsigned colonCount = 0;
			unsigned starCount = 0;
			for(auto it = str.begin(); it != str.end(); ++it)
			{
				if(*it == ':')
					colonCount++;
				if(*it == '*')
					starCount++;
			}
			if(starCount > colonCount/3)
			{
				splitEachByRegex(rtn, std::regex("\\n\\*"), true);
				trimEach(rtn, "\n");
			}
		}

		removeEmpty(rtn);
		flattenNewlines(rtn);
		return rtn;
	}

	std::list<Comment> parseLinesIntoComments(const std::list<std::string>& lines)
	{
		std::list<Comment> rtn;
		for(auto& line : lines)
			rtn.push_back(parseCommentFromLine(line));

		std::size_t outdent = 0;

		for(auto it = rtn.begin(); it != rtn.end(); it++)
		{
			// outdent detection 1
			std::regex odRegex1("/qqqOUTDENTqqq/");
			std::smatch ma;    // same as std::match_results<string::const_iterator> sm;
			bool flag = std::regex_search(it->Text,ma,odRegex1);
			if(flag && it != rtn.begin()) 
			{
				std::list<Comment>::iterator copyIt = it;
				copyIt--;
				if((copyIt->Text)[0] == ':')
				{
					std::size_t i = 0;
					while(i < copyIt->Text.size() && (copyIt->Text)[i] == ':')
						i++;
					outdent = i+1;
				}
				if((copyIt->Text)[0] == '*')
				{
					std::size_t i = 0;
					while(i < copyIt->Text.size() && (copyIt->Text)[i] == '*')
						i++;
					outdent = i+1;
				}
			}

			// outdent detection 2
			std::regex odRegex2("\\/qqqOUTDENTqqq([0-9]+)\\/");
			std::smatch ma2;    // same as std::match_results<string::const_iterator> sm;
			flag = std::regex_search(it->Text,ma2,odRegex2);
			if(flag) 
			{
				outdent += std::stoi(ma2[1]);
			}

			if(outdent > 0)
			{
				std::string toAdd = ":";
				std::list<Comment>::iterator copyIt = it;
				copyIt--;
				if(it != rtn.begin() && copyIt->Text[0] == '*')
					toAdd = "*";
				std::string tmp = "";
				for(auto a = 0; a < outdent; a++)
					tmp += toAdd;
				it->Text = tmp + it->Text;
			}

			if(it != --rtn.end() && it->User.empty()) // only if there comes something after the comment && user is empty -> check if there is only a signature in the following line
			{
				std::list<Comment>::iterator copyIt = it;
				std::string followingLine = (++copyIt)->Text; 
				boost::trim_if(followingLine,boost::is_any_of(":*"));

				std::vector<std::string> USER_REG_EXPS;
				USER_REG_EXPS.insert(USER_REG_EXPS.end(), STRONG_USER_REG_EXPS.begin(), STRONG_USER_REG_EXPS.end());
				USER_REG_EXPS.insert(USER_REG_EXPS.end(), WEAK_USER_REG_EXPS.begin(), WEAK_USER_REG_EXPS.end());

				RegexCombinatoricalParser combiParser({ { "-+" }, USER_REG_EXPS, FILLER_REG_EXP, DATE_REG_EXPS, { LINE_END_REG_EXP } });
				auto result = combiParser(followingLine);
				if(result.Matched)
				{
					it->User = result.Matches[1];
					it->DateString = result.Matches[2];
					it->Text += copyIt->Text;
					rtn.erase(copyIt);
				}

				RegexCombinatoricalParser combiParser2({ { "-+" }, USER_REG_EXPS, { LINE_END_REG_EXP } });
				auto result2 = combiParser2(followingLine);
				if(!result.Matched && result2.Matched)
				{
					it->User = result2.Matches[1];
					it->Text += copyIt->Text;
					rtn.erase(copyIt);
				}
			}
		}

		return rtn;
	}

	// ------------------- helper functions ---------------------------------

	void splitEachByRegex(std::list<std::string>& strs, const std::regex splitRegexStd, bool cutInFront)
	{
		std::list<std::string> tmp2;
		for(auto it = strs.begin(); it != strs.end(); ++it)
		{
			std::string strToSplit = *it;

			// find positions of all occurances
			auto content_begin = std::sregex_iterator(strToSplit.begin(), strToSplit.end(), splitRegexStd);
			auto content_end = std::sregex_iterator();
			std::sregex_iterator i = content_begin;
			std::vector<std::size_t> positions;
			for(auto innerIt = content_begin; innerIt != content_end; ++innerIt)
			{
				if(cutInFront)
					positions.push_back((std::size_t) innerIt->position());
				else
					positions.push_back((std::size_t) innerIt->position() + innerIt->length());
			}

			std::list<std::string> tmp;
			std::size_t lastCut = 0;
			for(auto posIt = positions.begin(); posIt != positions.end(); ++posIt)
			{
				auto pos = *posIt;
				tmp.push_back(strToSplit.substr(lastCut,pos-lastCut));
				lastCut = pos;
			}
			tmp.push_back(strToSplit.substr(lastCut,strToSplit.size()-lastCut));

			tmp2.insert(tmp2.end(), tmp.begin(), tmp.end());
		}
		strs = tmp2;
	}

	void trimEach(std::list<std::string>& strs, std::string charsToRemove)
	{
		for(auto it = strs.begin(); it != strs.end(); ++it)
		{
			boost::trim_if(*it,boost::is_any_of(charsToRemove));
		}
	}

	void removeEmpty(std::list<std::string>& strs)
	{
		for(auto it = strs.begin(); it != strs.end(); ++it)
		{
			if(*it == "")
				strs.erase(it++);
		}
	}

	void flattenNewlines(std::list<std::string>& strs)
	{
		const std::regex newlineRegex("\n");
		for(auto it = strs.begin(); it != strs.end(); ++it)
		{
			*it = std::regex_replace(*it, newlineRegex, "\\n");
		}
	}
}
