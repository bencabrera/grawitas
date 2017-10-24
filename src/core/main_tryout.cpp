#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "parsing/grammars/sections/sectionGrammar.hpp"
#include "parsing/grammars/sections/outdentGrammar.hpp"
#include "parsing/grammars/preprocessing/htmlCleaningGrammar.hpp"
#include "parsing/coreTalkPageParsing.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
		throw std::logic_error("Please provide file path");

	std::string path = argv[1];
	std::ifstream file(path);

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 


	// Grawitas::OutdentGrammar<std::string::const_iterator> outdentGrammar;
	// std::vector<std::tuple<std::string, int>> outdents;
	 // auto content_it = content.cbegin();
	// boost::spirit::qi::parse(content_it, content.cend(), outdentGrammar, outdents);

	// for(auto& outdent : outdents)
	// {
		// std::cout << "--------------------------" << std::endl;
		// std::cout << std::get<1>(outdent) << std::endl;
		// std::cout << "--------------------------" << std::endl;
		// std::cout << std::get<0>(outdent) << std::endl;
		// std::cout << "==========================" << std::endl;
	// }

	// // clean syntax of html and stuff
	// auto content_it = content.cbegin();
	// std::string result;
	// Grawitas::HtmlCleaningGrammar<std::string::const_iterator> htmlGrammar;
	// boost::spirit::qi::parse(content_it, content.cend(), htmlGrammar, result);

	// // split into sections
	// auto result_it = result.cbegin();
	// std::vector<std::tuple<std::string, std::string>> sections;
	// Grawitas::SectionGrammar<std::string::const_iterator, boost::spirit::qi::blank_type> sectionGrammar;
	// boost::spirit::qi::phrase_parse(result_it, result.cend(), sectionGrammar, boost::spirit::qi::blank, sections);

	// std::cout << "--------------------------" << std::endl;
	// std::cout << "sections.size(): " << sections.size() << std::endl;
	// std::cout << "--------------------------" << std::endl;
	// std::cout << "--------------------------" << std::endl;
	// std::cout << "--------------------------" << std::endl;

	// // split into outdents
	// std::vector<std::tuple<std::string, std::string, int>> all;
	// Grawitas::OutdentGrammar<std::string::const_iterator> outdentGrammar;
	// for(auto sec : sections)
	// {
		// auto& str = std::get<1>(sec);
		// auto str_it = str.cbegin();
		// std::vector<std::tuple<std::string, int>> outdents;
		// boost::spirit::qi::parse(str_it, str.cend(), outdentGrammar, outdents);

		// for(auto& outdent : outdents)
		// {
			// all.push_back({ std::get<0>(sec), std::get<0>(outdent), std::get<1>(outdent) });
		// }
	// }
	//
	
	auto all = Grawitas::split_into_sections(content);

	for(auto sec : all)
	{
		std::cout << "--------------------------" << std::endl;
		std::cout << std::get<0>(sec) << std::endl;
		std::cout << std::get<2>(sec) << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::cout << std::get<1>(sec) << std::endl;
		std::cout << "==========================" << std::endl;
	}

	return 0;
}
