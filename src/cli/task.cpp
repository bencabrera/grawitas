#include "task.h"

#include <iostream>
#include <set>
#include <chrono>
#include <iomanip>
#include "crawlerThread.h"

Task::Task(QObject *parent, const boost::program_options::variables_map& new_vm)
        : QObject(parent),
          vm(new_vm)
{}

void Task::run()
{
    // Do processing here

    if (!vm.count("talk-page-list-file")) {
        std::cout << "Please specify a file containing a list of articles for which the talk pages should be extracted." << std::endl;
        return;
    }

    if (!vm.count("output-folder")) {
        std::cout << "Please specify a output folder." << std::endl;
        return;
    }

    QString input_file = QString::fromStdString(vm.at("talk-page-list-file").as<std::string>());
    QString output_folder = QString::fromStdString(vm.at("output-folder").as<std::string>());
    std::set<Grawitas::Format> formats = { Grawitas::COMMENT_LIST_JSON };
    std::set<CrawlerOptions> options = {};

    CrawlerThread crawler(input_file, output_folder, formats, options);
    connect(&crawler, SIGNAL(write_status(std::string)), this, SLOT(print_status(std::string)));
    crawler.run();

    emit finished();
}

void Task::print_status(std::string msg)
{
 	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "[" << std::put_time(std::localtime(&now_c), "%F %T") << "]: ";
    std::cout << msg << std::endl;
}
