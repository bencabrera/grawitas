#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include "../talkPageParser/models.h"
#include "../output/formats.h"
#include "../output/outputWrapper.h"
#include "../httpCrawler/getPagesFromWikipedia.h"
#include "../talkPageParser/parsing.h"



void export_finished_pages(const std::string& output_folder, const std::set<Grawitas::Format> formats, const std::string& title, const std::vector<Grawitas::Comment>& parsed_talk_page, const std::string& dateToSplit);




