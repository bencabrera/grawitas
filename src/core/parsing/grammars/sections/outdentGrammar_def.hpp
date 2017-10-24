#pragma once

#define BOOST_SPIRIT_DEBUG

#include "outdentGrammar.hpp"

namespace Grawitas {

	template<typename Iterator>
		OutdentGrammar<Iterator>::OutdentGrammar() : OutdentGrammar::base_type(start, "OutdentGrammar") {
			using namespace boost::spirit::qi;
			using namespace boost::phoenix;
			using boost::phoenix::push_back;

			section = eps [at_c<1>(_val) = -1] >> -outdent [at_c<1>(_val) = boost::spirit::_1] >> *(!outdent >> char_) [at_c<0>(_val) += boost::spirit::_1] ;

			outdent_colons = +(lit(':') [_val += 1]);
			outdent = *space >> lit("{{") >> *blank >> outdent_labels >> *blank >> 
						-( '|' >> *blank >> 
							(
							 	outdent_colons [_val = boost::spirit::_1] 
								| ( int_ [_val = boost::spirit::_1] >> -('|' >> *blank >> lit("link")) ) 
							) 
						 )
					  >> *blank >> lit("}}");
			outdent_labels = "Outdent", "outdent", "od", "Od";

			start = section [push_back(_val, boost::spirit::_1)] % space;
		}

}
