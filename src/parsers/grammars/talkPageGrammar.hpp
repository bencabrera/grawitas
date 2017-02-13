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
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>

#include "commentGrammar.hpp"
#include "commentEndingGrammar.hpp"

namespace WikiTalkNet {

	template<typename Iterator, typename Skipper>
		struct TalkPageGrammar : boost::spirit::qi::grammar<Iterator, std::list<Comment>(), Skipper> {
			TalkPageGrammar();

			CommentGrammar<Iterator, Skipper> comment_grammar;
			CommentEndingGrammar<Iterator, Skipper> comment_ending;

			boost::spirit::qi::rule<Iterator,std::list<Comment>(), Skipper> comments;
		};

}
