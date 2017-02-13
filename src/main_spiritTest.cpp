#define BOOST_SPIRIT_USE_PHOENIX_V3 1
#define BOOST_SPIRIT_DEBUG

#define BOOST_SPIRIT_UNICODE // We'll use unicode (UTF8) all throughout

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_repeat.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <boost/spirit/include/phoenix.hpp>

#include <boost/fusion/adapted/std_pair.hpp>

#include <iomanip>
#include <fstream>
#include <iterator>
#include <utility>
#include <ctime>


namespace qi = boost::spirit::qi;


int main()
{
	// std::string str = "→";
	std::string str = "\u2192";
	// std::string str = "a";
	// std::string str = "0ab034";
	qi::rule<std::string::iterator, std::string(), qi::blank_type> r;
	// r = +(qi::unicode::char_ - '0') >> +qi::char_("0-9");
	// r = qi::unicode::char_(L'→');
	r = qi::lit("\u2192");
	// r = qi::standard_wide::char_(L'\u2192');

	auto start = str.begin();
	auto end = str.end();

	qi::phrase_parse(start, end, r, qi::blank);

	std::cout << "Original string: " << str << std::endl;

	std::cout << "Hallo" << std::endl;
	if(start == end)
		std::cout << "Matched" << std::endl;
	else
		std::cout << "NOT Matched" << std::endl;
	std::cout << "Hallo2" << std::endl;

	return 0;
}



