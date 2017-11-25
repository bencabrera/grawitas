#pragma once

#include "models.h"

#include "../core/output/formats.h"
#include "../core/models/parsedTalkPage.h"
#include "../core/parsing/coreTalkPageParsing.h"
#include "../core/output/outputWrapper.h"

constexpr int N_PAGES_PER_REQUEST = 2;
constexpr int N_NEXT_ARCHIVES = 2;
static_assert(N_PAGES_PER_REQUEST >= N_NEXT_ARCHIVES, "N_PAGES_PER_REQUEST has to larger equal to N_NEXT_ARCHIVES");

#include <set>

void crawling(const std::vector<std::string>& article_titles, const std::string& output_folder, const std::set<Grawitas::Format> formats);

std::vector<std::string> generate_next_titles_to_get(const std::vector<TalkPageResult>& results);
