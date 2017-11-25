#pragma once

#include <vector>
#include <string>

#include "models.h"

std::vector<TalkPageResult> request_raw_pages_from_wikipedia(std::vector<std::string> page_titles);
