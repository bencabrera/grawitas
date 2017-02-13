#pragma once

#define BOOST_SPIRIT_DEBUG

#include "dateGrammar.hpp"
#include "userGrammar.hpp"

#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <boost/fusion/adapted/std_pair.hpp>

#include <iostream>

namespace WikiTalkNet {

	template<typename Iterator, typename Skipper>
		struct SignatureGrammar : boost::spirit::qi::grammar<Iterator, std::pair<std::string, std::tm>(), Skipper>	{

			SignatureGrammar();

			boost::spirit::qi::rule<Iterator, std::pair<std::string, std::tm>(), Skipper> signature;

			UserGrammar<Iterator, Skipper> user_grammar;
			DateGrammar<Iterator, Skipper> date_grammar;

		};

}
