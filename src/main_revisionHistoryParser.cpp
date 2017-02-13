#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <iomanip>

#include "../libs/tinyxpath/tinyxml.h"
#include "../libs/tinyxpath/xpath_static.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace std;

namespace po = boost::program_options;

struct Edit{
    std::string Date;
    std::string User;
    std::string PageSize;
    std::string PlusMinus;
	bool IsMinor;
};


int main(int argc, char** argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "Produce help message.")
            ("html-input-file", po::value<string>(), "Path to the html input file.")
            ("formatted-txt-output-file", po::value<string>(), "Path for a txt file that contains the structured parsed data.")
            ("csv-output-file", po::value<string>(), "Path for a csv file that contains the parsed data in csv-format (semicolon-separated).")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
        return 0;
    }

    if (!vm.count("html-input-file")) {
        cout << "The parameter --html-input-file was not specified. Please specify an html file is needed as input! For displaying a parameter description please use --help." << std::endl;
        return 1;
    }

    if (!vm.count("formatted-txt-output-file") && !vm.count("csv-output-file")) {
        cout << "The parameters --formatted-txt-output-file and --csv-output-file were both not specified. Please specify some form of output. For displaying a parameter description please use --help." << std::endl;
        return 1;
    }

    TiXmlDocument doc;
    if( !doc.LoadFile(vm["html-input-file"].as<string>().c_str()) ){
        std::cout << "Could not open the file specified in --html-input-file.";
        return 1;
    }

    std::vector<Edit> parsedEdits;

    auto processor = TinyXPath::xpath_processor(doc.RootElement(), "//ul[@id='pagehistory']/*");
    auto res = processor.er_compute_xpath();
    auto node_set = res.nsp_get_node_set();

    for(unsigned i = 0; i < node_set->u_get_nb_node_in_set(); i++)
    {
        Edit curEdit;
        auto node = node_set->XNp_get_node_in_set(i);
        auto processor_date = TinyXPath::xpath_processor(node,"//a[@class='mw-changeslist-date']/text()");
        curEdit.Date = std::string(processor_date.S_compute_xpath().c_str());
        auto processor_user = TinyXPath::xpath_processor(node,"//span[@class='history-user']/a/bdi/text()");
        curEdit.User = std::string(processor_user.S_compute_xpath().c_str());
        auto processor_bytes = TinyXPath::xpath_processor(node,"//span[@class='history-size']/text()");
        curEdit.PageSize = std::string(processor_bytes.S_compute_xpath().c_str());
        auto processor_pmn = TinyXPath::xpath_processor(node,"//*[contains(@class,'mw-plusminus-null') or contains(@class ,'mw-plusminus-neg') or contains(@class ,'mw-plusminus-pos')]/text()");
        curEdit.PlusMinus = std::string(processor_pmn.S_compute_xpath().c_str());
        auto processor_minor = TinyXPath::xpath_processor(node,"//*[@class='minoredit']");
		auto resultMinor = processor_minor.er_compute_xpath();
    	auto node_set_minor = resultMinor.nsp_get_node_set();
		curEdit.IsMinor = node_set_minor->u_get_nb_node_in_set() > 0;

        boost::trim(curEdit.Date);
        boost::trim(curEdit.User);
        boost::trim(curEdit.PageSize);
        boost::trim(curEdit.PlusMinus);

        parsedEdits.push_back(curEdit);
    }

    if (vm.count("formatted-txt-output-file")) {
        std::ofstream formatted_file(vm["formatted-txt-output-file"].as<string>());
        formatted_file
        << std::setw(30) << "-----------------------" << " | "
        << std::setw(30) << "-----------------------" << " | "
        << std::setw(20) << "--------------" << " | "
        << std::setw(10) << "--------" << " | " << std::endl;
        formatted_file
        << std::setw(30) << "Date" << " | "
        << std::setw(30) << "User" << " | "
        << std::setw(20) << "PageSize" << " | "
        << setw(10) << "PlusMinus" << " | "
        << setw(5) << "IsMinorEdit" << " | "
        << std::endl;
        formatted_file
        << std::setw(30) << "-----------------------" << " | "
        << std::setw(30) << "-----------------------" << " | "
        << std::setw(20) << "--------------" << " | "
        << std::setw(10) << "--------" << " | "
        << std::setw(10) << "-----" << " | ";
        for(auto it = parsedEdits.begin(); it != parsedEdits.end(); ++it)
        {
            formatted_file
            << std::endl
            << std::setw(30) << it->Date << " | "
            << std::setw(30) << it->User << " | "
            << std::setw(20) << it->PageSize << " | "
            << setw(10) << it->PlusMinus << " | "
            << setw(5) << (it->IsMinor ? "Yes" : "No") << " | ";
        }
    }

    if (vm.count("csv-output-file")) {
        std::ofstream csv_output_file(vm["csv-output-file"].as<string>());
        for(auto it = parsedEdits.begin(); it != parsedEdits.end(); ++it)
        {
			auto pageSize = it->PageSize;
			boost::trim_if(pageSize,boost::is_any_of("()"));
			if(pageSize == "empty")
				pageSize = "0";
			boost::replace_all(pageSize, ",", "");
			boost::replace_all(pageSize, "bytes", "");
			boost::trim(pageSize);
			auto pm = it->PlusMinus;
			boost::trim_if(pm,boost::is_any_of("()"));
			boost::replace_all(pm, ",", "");

		 	csv_output_file
                << it->Date << ";"
                << it->User << ";"
                << pageSize << ";"
                << pm << ";"
				<< (it->IsMinor ? "Yes" : "No")
				<< std::endl;
        }
    }

    return 0;
}
