#include "coreTalkPageParsing.h"

#include <iostream>
#include <list>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <string>
#include <iomanip>
#include <streambuf>
#include <stack>

#include "grammars/sectionGrammar.hpp"
#include "grammars/talkPageGrammar.hpp"

namespace Grawitas {

	ParsedTalkPage parseTalkPage(std::istream& ostr)
	{
		// we read the file into a string first. 
		// because boost.spirit needs backtracking it will not be able to completely read from stream without keeping copies. Since our files should be relatively small (< 10MB) this should not be a limitation
		std::string content((std::istreambuf_iterator<char>(ostr)), std::istreambuf_iterator<char>()); 

		return parseTalkPage(content);
	}

	ParsedTalkPage parseTalkPage(const std::string& content)
	{
		// first split wikisyntax into sections
		auto content_it = content.cbegin();
		std::vector<std::tuple<std::string, std::string, int>> sections;
		Grawitas::SectionGrammar<std::string::const_iterator, boost::spirit::qi::blank_type> sectionGrammar;
		boost::spirit::qi::phrase_parse(content_it, content.cend(), sectionGrammar, boost::spirit::qi::blank, sections);
		
		// for each section apply now the comment parsing
		ParsedTalkPage parsedTalkPage;
		Grawitas::TalkPageGrammar<std::string::const_iterator, boost::spirit::qi::blank_type> talkPageGrammar;

		std::size_t currentSectionOutdent = 0;
		std::size_t lastCommentIndent = 0;

		for (auto sec : sections) {
			std::list<Comment> parsedSection;
			std::string tmpStr = std::get<1>(sec);

			auto section_it = tmpStr.cbegin();
			boost::spirit::qi::phrase_parse(section_it, tmpStr.cend(), talkPageGrammar, boost::spirit::qi::blank, parsedSection);

			int outdent = std::get<2>(sec);
			if(outdent == -1) {
				currentSectionOutdent = 0; // reset outdent
			}	
			else if(outdent == 0) {
				// outdent without parameters = every comment from now on until a section without outdent is indented a far as previous last comment was
				currentSectionOutdent = lastCommentIndent+1; 			
			} else {
				// indent further by the amount that was specified in {{outdent|amount}}
				currentSectionOutdent = currentSectionOutdent + outdent; 
			}

			for (auto& c : parsedSection) 
				c.IndentLevel += currentSectionOutdent;	

			lastCommentIndent = parsedSection.back().IndentLevel;
			if(outdent == -1 || parsedTalkPage.size() == 0)
				parsedTalkPage.push_back({ { std::get<0>(sec) }, parsedSection });
			else
			{
				auto& last = parsedTalkPage.back();
				last.second.insert(last.second.end(), parsedSection.begin(), parsedSection.end());
			}
		}

		return parsedTalkPage;
	}

	void calculateIds(std::list<Comment>& comments, std::size_t& startId)
	{
		std::list<std::size_t> refComments;
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
				comment.ParentId = refComments.front();
			}

			// if the colon count is smaller than size of stack we have to remove elements until we are at the correct level
			else if(comment.IndentLevel < refComments.size())
			{
				for(std::size_t i = 0; i < refComments.size()-comment.IndentLevel; i++)
					refComments.pop_front();
				comment.ParentId = refComments.front();
			}

			// if the colon count is larger than the size of the stack -> people did not indent correctly
			// -> add current top element as often as necessary to get to correct current colon level
			else if(comment.IndentLevel > refComments.size())
			{
				for(std::size_t i = 0; i < comment.IndentLevel-refComments.size(); i++)
					refComments.push_front(refComments.front());
				comment.ParentId = refComments.front();
			}

			refComments.push_front(comment.Id);
			startId++;
		}
	}

}
