#pragma once

#include "../models/comment.h"
#include <string>
#include <map>
#include <set>
#include <vector>


namespace WikiTalkNet {
	namespace Tests {

		std::tuple<std::map<std::size_t, std::size_t>, std::set<std::size_t>> computeOptimalTextMapping(const std::vector<WikiTalkNet::Comment>& annotation, const std::vector<WikiTalkNet::Comment>& parsed);

	}
}
