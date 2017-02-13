#include <iostream>
#include <sstream>
#include <vector>

// WARNING: very quick hacky solution...
namespace WikiTalkNet {
	namespace Tests {

		std::vector<std::size_t> findAllOccurances(const std::string& str, const char delim)
		{
			std::vector<std::size_t> rtn;

			for(std::size_t i = 0; i < str.length(); i++)
			{
				if(str[i] == delim)
					rtn.push_back(i);
			}

			return rtn;
		}

		std::vector<std::string> parseLine(const std::string& str)
		{
			auto commas = findAllOccurances(str, ',');	
			auto quotes = findAllOccurances(str, '"');	
			std::vector<std::size_t> cleaned_quotes;

			// leave out doubles quotation marks
			for(std::size_t i = 0; i < quotes.size(); i++)
			{
				if(str[quotes[i]] + 1 == str[quotes[i+1]])
					i++;
				else
					cleaned_quotes.push_back(quotes[i]);
			}



		}

		std::vector<std::vector<std::string>> readExcelCsv(std::istream& istr)
		{
			std::vector<std::vector<std::string>> rtn;

			std::string line;
			while(std::getline(istr, line))
			{
				rtn.push_back(parseLine(line));	
			}

			return rtn;
		}

	}
}
