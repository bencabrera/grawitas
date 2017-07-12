#pragma once

#define BOOST_SPIRIT_DEBUG

#include "sectionGrammar.hpp"

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		SectionGrammar<Iterator, Skipper>::SectionGrammar() : SectionGrammar::base_type(start, "SectionGrammar") {
			using namespace boost::spirit::qi;
			using namespace boost::phoenix;
			using boost::phoenix::push_back;

			heading = char_('=') >> char_('=') >> -char_('=') >> heading_text [_val = boost::spirit::qi::_1] >> -html_comment_text >> heading_end;
			heading_text = *(char_ - '=') [_val += boost::spirit::qi::_1] >> -(!heading_end >> char_('=') [_val += boost::spirit::qi::_1] >> heading_text [_val += boost::spirit::qi::_1]);
			heading_end = char_('=') >> char_('=') >> -char_('=') >> *blank >> eol;

			html_element = lit('<') >> -char_('/') >> html_element_name >> *(char_ - '>' - '\n') >> '>';
			html_comment_text = *(char_ - '-' ) >> -(!lit("-->") >> lit('-') >> html_comment_text);
			html_comment = lit("<!--") >> html_comment_text >> lit("-->");
			html_element_name = "span", "small", "large", "sup", "sub", "font", "ref";

			section_text = no_skip[*(char_ - '=' - '<' - '{') [_val += boost::spirit::qi::_1]]
			>> -(
				!outdent >> ((html_element |  html_comment | (!heading >> char_("=<{") [_val += boost::spirit::qi::_1]) ) >> section_text [_val += boost::spirit::qi::_1])
			);

			section = (outdent [at_c<2>(_val) = boost::spirit::_1] | heading [at_c<0>(_val) = boost::spirit::qi::_1, at_c<2>(_val) = -1]) >> section_text [at_c<1>(_val) = boost::spirit::qi::_1];

			outdent_colons = +(lit(':') [_val += 1]);
			outdent = lit("{{") >> outdent_labels >> 
						-( lit('|') >> 
								(
								 	outdent_colons [_val = boost::spirit::_1] 
									| ( int_ [_val = boost::spirit::_1] >> -(lit('|') >> lit("link")) ) 
								) 
						 )
					  >> lit("}}");
			outdent_labels = "Outdent", "outdent", "od", "Od";

			start = -section_text [at_c<1>(boost::phoenix::ref(helper_section)) = boost::spirit::qi::_1, at_c<2>(boost::phoenix::ref(helper_section)) = -1, push_back(_val, boost::phoenix::ref(helper_section))] >> section [push_back(_val, boost::spirit::_1)] % (-eol);

			start.name("SectionGrammar::Start");
			heading.name("SectionGrammar::Heading");
			heading_text.name("SectionGrammar::HeadingText");
			heading_end.name("SectionGrammar::HeadingEnd");

			html_element.name("SectionGrammar::HTMLElement");
			html_comment.name("SectionGrammar::HTMLComment");
			html_comment_text.name("SectionGrammar::HTMLCommentText");

			section_text.name("SectionGrammar::SectionText");
			section.name("SectionGrammar::Section");

			// BOOST_SPIRIT_DEBUG_NODE(section);
		}

}
