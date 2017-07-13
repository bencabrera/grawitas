#include "task.h"

#include <iostream>
#include <set>
#include <chrono>
#include <iomanip>

Task::Task(QObject *parent, const boost::program_options::variables_map& vm)
        : QObject(parent),
		  vm(vm),
          formats(formats_from_parameters(vm)),
		  options(options_from_parameters(vm))
{}


std::set<Grawitas::Format> Task::formats_from_parameters(const boost::program_options::variables_map& vm)
{
	std::set<Grawitas::Format> formats;

	for (auto form_parameter : Grawitas::FormatParameterStrings) 
		if(vm.count(form_parameter) > 0 && vm[form_parameter].as<bool>())
			formats.insert(Grawitas::parameter_to_format(form_parameter));

	return formats;
}

std::set<CrawlerOptions> Task::options_from_parameters(const boost::program_options::variables_map& vm)
{
	if(vm.count("keep-raw-talk-pages"))
		return { KEEP_TALK_PAGE_FILES };
	else 
		return std::set<CrawlerOptions>();
}

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
