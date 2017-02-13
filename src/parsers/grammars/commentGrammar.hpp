#pragma once

#define BOOST_SPIRIT_DEBUG

#include "signatureGrammar.hpp"
#include "commentEndingGrammar.hpp"
#include "../../models/comment.h"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <iostream>


BOOST_FUSION_ADAPT_STRUCT(
	WikiTalkNet::Comment,
	(std::string, Text)
	(std::string, User)
	(std::tm, Date)
	(std::size_t, IndentLevel)
)

namespace WikiTalkNet {

	template<typename Iterator, typename Skipper>
		struct CommentGrammar : boost::spirit::qi::grammar<Iterator, Comment(), Skipper> {

			CommentGrammar();

			boost::spirit::qi::rule<Iterator,Comment(), Skipper> start;
			boost::spirit::qi::rule<Iterator, std::size_t(), Skipper> indentation;
			boost::spirit::qi::rule<Iterator, std::string(), Skipper> text;

			SignatureGrammar<Iterator, Skipper> signature;
			CommentEndingGrammar<Iterator, Skipper> comment_ending;

		};

}
