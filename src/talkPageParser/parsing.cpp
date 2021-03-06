#include "parsing.h"

#include <iostream>
#include <list>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <string>
#include <iomanip>
#include <streambuf>
#include <stack>

#include "grammars/sections/outdentGrammar.hpp"
#include "grammars/sections/sectionGrammar.hpp"
#include "grammars/preprocessing/htmlCleaningGrammar.hpp"
#include "grammars/talkPageGrammar.hpp"

namespace Grawitas {

	std::string clean_content(const std::string& content)
	{
		// clean syntax of html and stuff
		auto content_it = content.cbegin();
		std::string result;
		Grawitas::HtmlCleaningGrammar<std::string::const_iterator> htmlGrammar;
		boost::spirit::qi::parse(content_it, content.cend(), htmlGrammar, result);

		return result;
	}

	std::vector<std::tuple<std::string, std::string, int>> split_section_into_outdents(const std::tuple<std::string, std::string>& sec)
	{
		std::vector<std::tuple<std::string, std::string, int>> rtn;

		Grawitas::OutdentGrammar<std::string::const_iterator> outdentGrammar;

		auto& str = std::get<1>(sec);
		auto str_it = str.cbegin();
		std::vector<std::tuple<std::string, int>> outdents;

		try {
			boost::spirit::qi::parse(str_it, str.cend(), outdentGrammar, outdents);
		}
		catch(boost::spirit::qi::expectation_failure<std::string::const_iterator> exp)
		{}

		for(auto& outdent : outdents)
			rtn.push_back(std::tuple<std::string, std::string, int>{ std::get<0>(sec), std::get<0>(outdent), std::get<1>(outdent) });

		return rtn;
	}


	std::vector<std::tuple<std::string, std::string, int>> split_into_sections(const std::string& uncleaned_content)
	{
		std::string content = clean_content(uncleaned_content);

		std::vector<std::tuple<std::string, std::string>> sections;

		// first split wikisyntax into sections
		auto content_it = content.cbegin();
		Grawitas::SectionGrammar<std::string::const_iterator, boost::spirit::qi::iso8859_1::blank_type> sectionGrammar;
		try {
			boost::spirit::qi::phrase_parse(content_it, content.cend(), sectionGrammar, boost::spirit::qi::iso8859_1::blank, sections);
		}
		catch(boost::spirit::qi::expectation_failure<std::string::const_iterator> exp)
		{}

		// remove empty sections
		sections.erase(std::remove_if(sections.begin(), sections.end(), [](const std::tuple<std::string, std::string>& t) {
			return std::get<1>(t).empty();
		}), sections.end());

		std::vector<std::tuple<std::string, std::string, int>> rtn;
		for(auto& sec : sections)
		{
			auto outdents = split_section_into_outdents(sec);
			rtn.insert(rtn.end(), outdents.begin(), outdents.end());
		}

		return rtn;
	}

	std::vector<Comment> parse_one_section(const std::string& section_content, const int outdent, std::size_t& current_section_outdent, const std::size_t last_comment_level)
	{
		// for each section apply now the comment parsing
		static Grawitas::TalkPageGrammar<std::string::const_iterator, boost::spirit::qi::iso8859_1::blank_type> talkPageGrammar;

		std::vector<Comment> parsed_section;
		try {
			boost::spirit::qi::phrase_parse(section_content.cbegin(), section_content.cend(), talkPageGrammar, boost::spirit::qi::iso8859_1::blank, parsed_section);
		}
		catch(boost::spirit::qi::expectation_failure<std::string::const_iterator> exp)
		{}

		if(outdent == -1) {
			current_section_outdent = 0; // reset outdent
		}	
		else if(outdent == 0) {
			// outdent without parameters = every comment from now on until a section without outdent is indented a far as previous last comment was
			current_section_outdent = last_comment_level+1; 			
		} else {
			// indent further by the amount that was specified in {{outdent|amount}}
			current_section_outdent = current_section_outdent + outdent; 
		}

		for (auto& c : parsed_section) 
			c.IndentLevel += current_section_outdent;	

		return parsed_section;
	}

	void calculate_ids(std::vector<Comment>& comments, std::size_t& startId)
	{
		std::vector<std::size_t> refComments;
		for(auto& comment : comments)
		{
			comment.Id = startId;

			// if there is nothing yet on the stack -> parent id can just stay at 0 because there is no parent
			if(comment.IndentLevel == 0)
			{
				comment.ParentId = 0;
				refComments.clear();
			}

			// if the colon count is equal to the number of elements on the stack -> the top-most element on the stack is parent of current comment 
			else if(comment.IndentLevel == refComments.size())
			{
				comment.ParentId = refComments.back();
			}

			// if the colon count is smaller than size of stack we have to remove elements until we are at the correct level
			else if(comment.IndentLevel < refComments.size())
			{
				for(std::size_t i = 0; i < refComments.size()-comment.IndentLevel; i++)
					refComments.pop_back();
				comment.ParentId = refComments.back();
			}

			// if the colon count is larger than the size of the stack -> people did not indent correctly
			// -> add current top element as often as necessary to get to correct current colon level
			else if(comment.IndentLevel > refComments.size())
			{
				if(refComments.size() == 0)
					comment.ParentId = 0;
				else {
					for (std::size_t i = 0; i < comment.IndentLevel - refComments.size(); i++)
						refComments.push_back(refComments.back());
					comment.ParentId = refComments.back();
				}
			}

			refComments.push_back(comment.Id);
			startId++;
		}
	}


	std::vector<Comment> parse_talk_page(const std::string& content)
	{
		std::vector<Comment> parsed_talk_page;

		// split everything into sections
		auto sections = split_into_sections(content);

		// parse each section
		std::size_t current_section_outdent = 0;
		std::size_t last_comment_level = 0;
		for (auto sec : sections) {

			auto& section_content = std::get<1>(sec);
			auto& section_outdent  = std::get<2>(sec);
			auto parsed_section = parse_one_section(section_content, section_outdent, current_section_outdent, last_comment_level);

			// if no comments could be parsed -> don't add to parsed talk page
			if(parsed_section.size() == 0)
				continue;

			// if section is outdent -> set .Section of comments to the same as previous section; otherwise set title of sec
			if(section_outdent != -1 && parsed_talk_page.size() != 0)
			{
				auto& last_section_title = parsed_talk_page.back().Section;
				std::for_each(parsed_section.begin(), parsed_section.end(), [&last_section_title](Comment& comment) { comment.Section = last_section_title; }); 
			}
			else
			{
				auto& section_title = std::get<0>(sec);
				std::for_each(parsed_section.begin(), parsed_section.end(), [&section_title](Comment& comment) { comment.Section = section_title; }); 
			}
			parsed_talk_page.insert(parsed_talk_page.end(), parsed_section.begin(), parsed_section.end());

			if(parsed_section.size() > 0)
				last_comment_level = parsed_section.back().IndentLevel;
			else
				last_comment_level = 0;
		}

		// calculate ids for each parsed section
		std::size_t cur_id = 1;
		calculate_ids(parsed_talk_page, cur_id);	
		std::for_each(parsed_talk_page.begin(), parsed_talk_page.end(), [](Comment& comment) { 
			boost::trim(comment.User); 
			if(!comment.User.empty())
				comment.User[0] = std::toupper(comment.User[0]);

			boost::replace_all(comment.User, "_", " ");
			boost::trim(comment.Section);
		}); // trim comment.User and comment.Section

		return parsed_talk_page;
	}


	std::vector<Comment> parse_talk_page(std::istream& ostr)
	{
		// we read the file into a string first. 
		// because boost.spirit needs backtracking it will not be able to completely read from stream without keeping copies. Since our files should be relatively small (< 10MB) this should not be a limitation
		std::string content((std::istreambuf_iterator<char>(ostr)), std::istreambuf_iterator<char>()); 

		return parse_talk_page(content);
	}

}
