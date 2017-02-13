#pragma once

#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_UNICODE

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
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

namespace WikiTalkNet {

	template<typename Iterator, typename Skipper>
		struct UserGrammar : public boost::spirit::qi::grammar<Iterator, std::string(), Skipper> {

			using S_RULE = boost::spirit::qi::rule<Iterator,std::string(), Skipper>;

			UserGrammar();

			S_RULE start;
			S_RULE user_related_link;
			S_RULE username_link;
			S_RULE ip_adress_username_link;
			S_RULE ip_adress;
			S_RULE talk_link;
			
			boost::spirit::qi::rule<Iterator> between_links_skippers;
		};

}
