#pragma once

// #define BOOST_SPIRIT_DEBUG 

#include "signatureGrammar.hpp"

namespace Grawitas {

	template<typename Iterator, typename Skipper>
		SignatureGrammar<Iterator, Skipper>::SignatureGrammar() : SignatureGrammar::base_type(signature, "SignatureGrammar") {
			using namespace boost::spirit::qi;
			using namespace boost::phoenix;

			signature = -char_('(') >> -lit("←") >> user_grammar [at_c<0>(_val) = boost::spirit::qi::_1] >> -lit("→") >> date_grammar [at_c<1>(_val) = boost::spirit::qi::_1] >> -char_(')');

			signature.name("SignatureGrammar::Signature");

			// BOOST_SPIRIT_DEBUG_NODE(signature);

			// on_error<fail>(signature, 
					// std::cout
					// << boost::phoenix::val("Error! Expecting ")
					// << boost::spirit::_4                               // what failed?
					// << boost::phoenix::val(" here: \"")
					// << boost::phoenix::construct<std::string>(boost::spirit::_3, boost::spirit::_2)   // iterators to error-pos, end
					// << boost::phoenix::val("\"")
					// << std::endl
					// );
		}

}
