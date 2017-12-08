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
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>

#include <iostream>
#include <vector>

namespace Grawitas {

	template<typename Iterator>
		struct OutdentGrammar : boost::spirit::qi::grammar<Iterator, std::vector<std::tuple<std::string, int>>()> {

			OutdentGrammar();

			boost::spirit::qi::rule<Iterator, std::vector<std::tuple<std::string, int>>()> start;

			boost::spirit::qi::rule<Iterator, std::tuple<std::string, int>()> section;

			boost::spirit::qi::rule<Iterator, int()> outdent;
			boost::spirit::qi::rule<Iterator, int()> outdent_colons;
			boost::spirit::qi::symbols<char> outdent_labels;
		};

}
