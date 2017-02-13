#pragma once

#define BOOST_SPIRIT_DEBUG

#include "commentEndingGrammar.hpp"

namespace WikiTalkNet {

	template<typename Iterator, typename Skipper>
		CommentEndingGrammar<Iterator, Skipper>::CommentEndingGrammar() : CommentEndingGrammar::base_type(comment_ending, "CommentEndingGrammar") {
			using namespace boost::spirit::qi;
			using boost::phoenix::push_back;

			comment_ending = *char_(".;") [_val += 1, _pass = _val < 5] >> (eol | eoi);
		}
}
