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

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		struct CommentEndingGrammar : boost::spirit::qi::grammar<Iterator, std::size_t(), Skipper> {
			CommentEndingGrammar();

			boost::spirit::qi::rule<Iterator, std::size_t(), Skipper> comment_ending;
		};

}
