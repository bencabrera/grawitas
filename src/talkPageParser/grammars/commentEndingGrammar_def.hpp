#pragma once

#define BOOST_SPIRIT_DEBUG

#include "commentEndingGrammar.hpp"

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		CommentEndingGrammar<Iterator, Skipper>::CommentEndingGrammar() : CommentEndingGrammar::base_type(comment_ending, "CommentEndingGrammar") {
			using namespace boost::spirit::qi;
			using boost::phoenix::push_back;
			using boost::spirit::qi::iso8859_1::char_;

			comment_ending = *char_(".;") [_val += 1, _pass = _val < 5] >> (eol | eoi);
		}
}
