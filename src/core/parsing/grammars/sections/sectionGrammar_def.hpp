#pragma once

#define BOOST_SPIRIT_DEBUG

#include "sectionGrammar.hpp"

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		SectionGrammar<Iterator, Skipper>::SectionGrammar() : SectionGrammar::base_type(start, "SectionGrammar") {
			using namespace boost::spirit::qi;
			using namespace boost::phoenix;
			using boost::phoenix::push_back;

			heading = repeat(2,3)['='] >> *blank >> heading_text [_val = boost::spirit::qi::_1] >> heading_end;
			heading_text = *(!heading_end >> !eol >> char_);
			heading_end = repeat(2,3)['='] >> *blank >> eol;

			section_text = *(!heading >> char_);

			section = (heading [at_c<0>(_val) = boost::spirit::qi::_1]) >> section_text [at_c<1>(_val) = boost::spirit::qi::_1];

			start = -section_text [at_c<1>(boost::phoenix::ref(helper_section)) = boost::spirit::qi::_1, push_back(_val, boost::phoenix::ref(helper_section))] >> section [push_back(_val, boost::spirit::_1)] % (-eol);
		}

}
