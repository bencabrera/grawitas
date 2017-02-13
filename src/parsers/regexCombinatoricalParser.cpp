#include "regexCombinatoricalParser.h"

namespace WikiTalkNet 
{
	RegexCombinatoricalParser::RegexCombinatoricalParser()
	{}

	RegexCombinatoricalParser::RegexCombinatoricalParser(std::vector<std::vector<std::string>> data)
		:_expressionArray(data)
	{}

	bool RegexCombinatoricalParser::nextCombination(std::vector<std::size_t>& combination)
	{
		std::size_t i = 0;
		while(i < combination.size()-1 && combination.at(i) == _expressionArray.at(i).size()-1)
		{
			combination.at(i) = 0;
			i++;
		}

		if(i == combination.size()-1 && combination.back() == _expressionArray.at(i).size()-1)
			return false;

		if(i< combination.size())
			combination.at(i)++;

		// reached last regex part
		return true;
	}

	std::vector<std::string> RegexCombinatoricalParser::getAllRegexStrings()
	{
		std::vector<std::string> rtn;
		std::vector<std::size_t> position(_expressionArray.size(), 0);
		do
		{
			std::string regexStr;
			for(std::size_t a = 0; a < position.size(); a++)
				regexStr += _expressionArray[a][position[a]];
			rtn.push_back(regexStr);
		}
		while(nextCombination(position));

		return rtn;
	}

	RegexCombinatoricalParser::Result RegexCombinatoricalParser::operator()(const std::string& str)
	{
		std::vector<std::size_t> position(_expressionArray.size(), 0);

		do
		{
			std::string regexStr;
			for(std::size_t a = 0; a < position.size(); a++)
				regexStr += _expressionArray[a][position[a]];

			std::regex regex(regexStr);
			std::smatch ma;    // same as std::match_results<string::const_iterator> sm;
			bool flag = std::regex_search(str,ma,regex);
			if(flag) {
				Result result;
				result.Matched = true;
				result.Matches = ma;
				result.Regex = regexStr;
				return result;
			}
		}
		while(nextCombination(position));

		Result result;
		result.Matched = false;
		return result;
	}


	void RegexCombinatoricalParser::addPart(std::vector<std::string> expressions)
	{
		_expressionArray.push_back(expressions);
	}
}
