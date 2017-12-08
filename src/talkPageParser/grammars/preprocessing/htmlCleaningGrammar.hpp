#pragma once

#define BOOST_SPIRIT_DEBUG


#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_repeat.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/variant/recursive_variant.hpp>

namespace Grawitas {

	template<typename Iterator>
		struct HtmlCleaningGrammar : boost::spirit::qi::grammar<Iterator, std::string()> {
			HtmlCleaningGrammar();

			boost::spirit::qi::rule<Iterator, std::string()> html_cleaning;

			boost::spirit::qi::rule<Iterator> html_element;
			boost::spirit::qi::rule<Iterator> html_comment_text;
			boost::spirit::qi::rule<Iterator> html_comment;
			boost::spirit::qi::symbols<char> html_element_name;

			boost::spirit::qi::rule<Iterator> html_special_characters;

			boost::spirit::qi::symbols<char> wiki_text_formatting;
		};

}
