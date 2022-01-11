#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>


namespace Grawitas {

    void export_finished_pages(const std::string& output_folder, const std::set<Grawitas::Format> formats, const std::string& title, const std::vector<Grawitas::Comment>& parsed_talk_page, std::string dateToSplit);

}



