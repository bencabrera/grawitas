#pragma once

#define BOOST_SPIRIT_DEBUG

#include "htmlCleaningGrammar.hpp"

namespace Grawitas {

	template<typename Iterator>
		HtmlCleaningGrammar<Iterator>::HtmlCleaningGrammar() : HtmlCleaningGrammar::base_type(html_cleaning, "HtmlCleaningGrammar") {
			using namespace boost::spirit::qi;

			html_element = lit('<') >> -char_('/') >> html_element_name >> *(char_ - '>' - '\n') >> '>';
			html_comment_text = *(char_ - '-' ) >> -(!lit("-->") >> lit('-') >> html_comment_text);
			html_comment = lit("<!--") >> html_comment_text >> lit("-->");
			html_element_name = "span", "small", "large", "sup", "sub", "font", "ref";

			html_special_characters = '&' >> (('#' >> repeat(2,7)[digit]) | repeat(2,7)[alnum]) >> ';';

			wiki_text_formatting = "''", "'''", "'''''";


			html_cleaning = 
			*(
				*(char_ - '<' - '\'' - '&') 
				>> (
					(&html_element >> html_element) 
					| (&html_comment >> html_comment) 
					| (&wiki_text_formatting >> wiki_text_formatting) 
					| (&html_special_characters >> html_special_characters) 
					| char_("<'&")
				)
			);
		}
}
