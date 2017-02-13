#include "optimalTextMapping.h"

#include <numeric>
#include <iostream>

#include <boost/algorithm/string.hpp>


namespace WikiTalkNet {
	namespace Tests {

		std::tuple<std::map<std::size_t, std::size_t>, std::set<std::size_t>> computeOptimalTextMapping(const std::vector<WikiTalkNet::Comment>& annotation, const std::vector<WikiTalkNet::Comment>& parsed)
		{
			std::map<std::size_t, std::size_t> mapping;
			std::set<std::size_t> mapped_to;
			std::set<std::size_t> not_found;
			for(auto i = 0; i < annotation.size(); i++)
				not_found.insert(i);

			for(std::size_t iAnno = 0; iAnno < annotation.size(); iAnno++)
			{
				for(std::size_t iParsed = 0; iParsed < parsed.size(); iParsed++)
				{
					std::string text1 = boost::to_lower_copy(boost::trim_copy(annotation[iAnno].Text));
					std::string text2 = boost::to_lower_copy(boost::trim_copy(parsed[iParsed].Text));

					if(text1 == text2)	// replace by approx. matching?! AND add matching of date and user name
					{
						if(mapped_to.find(iParsed) == mapped_to.end())
						{
							mapping.insert({ iAnno, iParsed });
							mapped_to.insert(iParsed);
							not_found.erase(iAnno);
						}
						else
							throw std::logic_error("Found text twice");
					}
				}
			}


			return std::tuple<std::map<std::size_t, std::size_t>, std::set<std::size_t>>{ mapping, not_found };
		}	

	}
}
