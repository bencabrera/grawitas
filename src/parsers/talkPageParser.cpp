#include "talkPageParser.h"

#include <iostream>
#include <list>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <string>
#include <iomanip>
#include <streambuf>
#include <stack>

#include "../parsers/grammars/sectionGrammar.hpp"
#include "../parsers/grammars/talkPageGrammar.hpp"

namespace WikiTalkNet {

	ParsedTalkPage parseTalkPage(std::istream& ostr)
	{
		std::string content((std::istreambuf_iterator<char>(ostr)), std::istreambuf_iterator<char>()); // todo parse directly from stream
		// std::cout << content << std::endl;

		// first split it into sections
		
		auto content_it = content.begin();
		std::vector<std::tuple<std::string, std::string, int>> sections;
		WikiTalkNet::SectionGrammar<std::string::iterator, boost::spirit::qi::blank_type> sectionGrammar;
		boost::spirit::qi::phrase_parse(content_it, content.end(), sectionGrammar, boost::spirit::qi::blank, sections);
		
		// for each section apply now the comment parsing
		ParsedTalkPage parsedTalkPage;
		WikiTalkNet::TalkPageGrammar<std::string::iterator, boost::spirit::qi::blank_type> talkPageGrammar;

		std::size_t currentSectionOutdent = 0;
		std::size_t lastCommentIndent = 0;

		for (auto sec : sections) {
			std::list<Comment> parsedSection;
			std::string tmpStr = std::get<1>(sec);
			// std::cout << std::endl << "--------------------------------" << std::endl << tmpStr << std::endl << std::endl << std::endl;

			auto section_it = tmpStr.begin();
			boost::spirit::qi::phrase_parse(section_it, tmpStr.end(), talkPageGrammar, boost::spirit::qi::blank, parsedSection);

			int outdent = std::get<2>(sec);
			if(outdent == -1) {
				currentSectionOutdent = 0; // reset outdent
			}	
			else if(outdent == 0) {
				currentSectionOutdent = lastCommentIndent+1; // outdent without parameters = every comment from now on until a section without outdent is indented a far as previous last comment was
			} else {
				currentSectionOutdent = currentSectionOutdent + outdent; // indent further by the amount that was specified in {{outdent|amount}}
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
