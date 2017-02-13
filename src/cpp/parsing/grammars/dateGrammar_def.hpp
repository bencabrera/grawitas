#pragma once

#include "dateGrammar.hpp"

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		DateGrammar<Iterator, Skipper>::DateGrammar() : DateGrammar::base_type(start, "DateGrammar") {
			using namespace boost::spirit::qi;
			using namespace boost::phoenix;


			using boost::phoenix::val;
			using boost::phoenix::construct;

			short_month_str.add
				("Jan", 0)
				("Feb", 1)
				("Mar", 2)
				("Apr", 3)
				("May", 4)
				("Jun", 5)
				("Jul", 6)
				("Aug", 7)
				("Sep", 8)
				("Oct", 9)
				("Nov", 10)
				("Dec", 11)
				;

			long_month_str.add
				("January", 0)
				("February", 1)
				("March", 2)
				("April", 3)
				("May", 4)
				("June", 5)
				("July", 6)
				("August", 7)
				("September", 8)
				("October", 9)
				("November", 10)
				("December", 11)
				;

			hour_and_second = int_ [at_c<0>(_val) = boost::spirit::_1] >> ':' >> int_ [at_c<1>(_val) += boost::spirit::_1];
			day_month_year = int_ [at_c<2>(_val) = boost::spirit::_1, _pass = boost::spirit::_1 < 32] >> (long_month_str | short_month_str) [at_c<1>(_val) = boost::spirit::_1] >> int_ [at_c<0>(_val) = boost::spirit::_1];
			year_month_day = int_ [at_c<0>(_val) = boost::spirit::_1, _pass = boost::spirit::_1 > 1900] >> (long_month_str | short_month_str) [at_c<1>(_val) = boost::spirit::_1] >> int_ [at_c<2>(_val) = boost::spirit::_1, _pass = boost::spirit::_1 < 32];
			month_day_year = (long_month_str | short_month_str ) [at_c<1>(_val) = boost::spirit::_1] >>  int_ [at_c<2>(_val) = boost::spirit::_1] >> char_(',') >> int_ [at_c<0>(_val) += boost::spirit::_1];
			start = 
				hour_and_second [at_c<3>(_val) = at_c<0>(boost::spirit::_1), at_c<4>(_val) = at_c<1>(boost::spirit::_1)] >> char_(',')  
				>> (day_month_year | month_day_year | year_month_day) [at_c<0>(_val) = at_c<0>(boost::spirit::_1) - 1900, at_c<1>(_val) = at_c<1>(boost::spirit::_1), at_c<2>(_val) = at_c<2>(boost::spirit::_1)] 
				>> string("(UTC)");

			// hour_and_second = int_ [_val += boost::spirit::_1] >> ':' >> int_ [_val += boost::spirit::_1];
			// day_month_year = int_ [_val += boost::spirit::_1] >> (long_month_str | short_month_str) [_val += boost::spirit::_1] >> int_ [_val += boost::spirit::_1];
			// month_day_year = (long_month_str | short_month_str ) [_val += boost::spirit::_1] >>  int_ [_val += boost::spirit::_1] >> char_(',') [_val += boost::spirit::_1] >> int_ [_val += boost::spirit::_1];
			// start = hour_and_second [_val += boost::spirit::_1] >> char_(',') [_val += boost::spirit::_1] >> (day_month_year | month_day_year) [_val += boost::spirit::_1] >> string("(UTC)") [_val += boost::spirit::_1];

			start.name("DateGrammar::Start");
			hour_and_second.name("DateGrammar::HourAndSecond");
			day_month_year.name("DateGrammar::DayMonthYear");
			short_month_str.name("DateGrammar::ShortMonthString");
			long_month_str.name("DateGrammar::LongMonthString");


			// on_error<fail>(start, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(hour_and_second, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(day_month_year, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);
			// on_error<fail>(month_day_year, std::cout << boost::phoenix::val("Error! Expecting ") << boost::spirit::_4 << boost::phoenix::val(" here: \"") << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2) << boost::phoenix::val("\"") << std::endl);

		}
}
