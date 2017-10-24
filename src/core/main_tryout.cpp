#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "parsing/grammars/sections/sectionGrammar.hpp"
#include "parsing/grammars/preprocessing/htmlCleaningGrammar.hpp"

int main(int argc, char *argv[])
{
	if(argc != 2)
		throw std::logic_error("Please provide file path");

	std::string path = argv[1];
	std::ifstream file(path);

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 



	// clean syntax of html and stuff
	auto content_it = content.cbegin();
	std::string result;
	Grawitas::HtmlCleaningGrammar<std::string::const_iterator> htmlGrammar;
	boost::spirit::qi::parse(content_it, content.cend(), htmlGrammar, result);

	// split into sections
	auto result_it = result.cbegin();
	std::vector<std::tuple<std::string, std::string>> sections;
	Grawitas::SectionGrammar<std::string::const_iterator, boost::spirit::qi::blank_type> sectionGrammar;
	boost::spirit::qi::phrase_parse(result_it, result.cend(), sectionGrammar, boost::spirit::qi::blank, sections);

	std::cout << "--------------------------" << std::endl;
	std::cout << "sections.size(): " << sections.size() << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "--------------------------" << std::endl;

	for(auto sec : sections)
	{
		std::cout << "--------------------------" << std::endl;
		std::cout << std::get<0>(sec) << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::cout << std::get<1>(sec) << std::endl;
		std::cout << "==========================" << std::endl;
	}

	return 0;
}
