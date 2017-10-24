#include <iostream>
#include <string>
#include <fstream>
#include <vector>
// #include "parsing/grammars/sectionGrammar.hpp"
#include "parsing/grammars/preprocessing/htmlCleaningGrammar.hpp"

int main(int argc, char *argv[])
{
	if(argc != 2)
		throw std::logic_error("Please provide file path");

	std::string path = argv[1];
	std::ifstream file(path);

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 


	// std::vector<std::tuple<std::string, std::string, int>> sections;

	// first split wikisyntax into sections
	auto content_it = content.cbegin();
	std::string result;

	Grawitas::HtmlCleaningGrammar<std::string::const_iterator> htmlGrammar;
	boost::spirit::qi::parse(content_it, content.cend(), htmlGrammar, result);

	std::cout << result;

	// Grawitas::SectionGrammar<std::string::const_iterator, boost::spirit::qi::blank_type> sectionGrammar;
	// boost::spirit::qi::phrase_parse(content_it, content.cend(), sectionGrammar, boost::spirit::qi::blank, sections);

	// std::cout << "--------------------------" << std::endl;
	// std::cout << "sections.size(): " << sections.size() << std::endl;
	// for(auto sec : sections)
		// std::cout << std::get<0>(sec) << " --- " << std::get<1>(sec) << std::endl;


	return 0;
}
