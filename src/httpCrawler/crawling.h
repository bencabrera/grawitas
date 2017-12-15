#pragma once

#include "../talkPageParser/models.h"

#include "../output/formats.h"
#include "../talkPageParser/models.h"
#include "../output/outputWrapper.h"

#include <set>
#include <functional>

namespace Grawitas {
	constexpr int N_PAGES_PER_REQUEST = 10;
	constexpr int N_NEXT_ARCHIVES = 10;
	static_assert(N_PAGES_PER_REQUEST >= N_NEXT_ARCHIVES, "N_PAGES_PER_REQUEST has to larger equal to N_NEXT_ARCHIVES");

	struct AdditionalCrawlerOptions {
		inline AdditionalCrawlerOptions()
		:keep_raw_talk_pages(false),
		abort(nullptr)
		{}

		bool keep_raw_talk_pages;
		bool* abort;
		std::function<void(const std::string&)> status_callback;
	};

	void crawling(std::vector<std::string> article_titles, const std::string& output_folder, const std::set<Grawitas::Format> formats, AdditionalCrawlerOptions options = AdditionalCrawlerOptions());
}
