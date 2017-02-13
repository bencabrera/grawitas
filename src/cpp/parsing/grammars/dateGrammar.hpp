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
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	std::tm,
	(int, tm_year)
	(int, tm_mon)
	(int, tm_mday)
	(int, tm_hour)
	(int, tm_min)
)

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		struct DateGrammar : boost::spirit::qi::grammar<Iterator, std::tm(), Skipper> {

			DateGrammar();

			boost::spirit::qi::rule<Iterator, std::tm(), Skipper> start;
			boost::spirit::qi::rule<Iterator, std::pair<int, int>()> hour_and_second;
			boost::spirit::qi::rule<Iterator, std::tuple<int, int, int>(), Skipper> day_month_year;
			boost::spirit::qi::rule<Iterator, std::tuple<int, int, int>(), Skipper> month_day_year;
			boost::spirit::qi::rule<Iterator, std::tuple<int, int, int>(), Skipper> year_month_day;

			boost::spirit::qi::symbols<char,int> short_month_str;
			boost::spirit::qi::symbols<char,int> long_month_str;
		};

}
