#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <regex>

namespace WikiTalkNet 
{
	/*
	 * \brief This class wraps the functionality of running all possible different concatinations of multiple regex-expressions until a match is found or all possibilities are tried
	 */ 
	class RegexCombinatoricalParser
	{
		public:
			struct Result {
				std::string Regex;
				std::smatch Matches; 
				bool Matched;
			};

			Result operator()(const std::string& str);
			bool nextCombination(std::vector<std::size_t>& combination);

			void addPart(std::vector<std::string> expressions);

			RegexCombinatoricalParser();
			RegexCombinatoricalParser(std::vector<std::vector<std::string>> data);

			std::vector<std::string> getAllRegexStrings();


		private:
			std::vector<std::vector<std::string>> _expressionArray;
	};


}
