#include "crawler_task.h"

CrawlerTask::CrawlerTask(QObject *parent, const std::vector<std::string>& titles, const std::string& output_folder, const std::set<Grawitas::Format>& formats)
	: QObject(parent),
	_titles(titles),
	_formats(formats),
	_output_folder(output_folder)
{}

void CrawlerTask::run(){
	crawling(_titles, _output_folder, _formats);

    // struct timespec ts = { 1000 / 1000, (1000 % 1000) * 1000 * 1000 };
    // nanosleep(&ts, NULL);

	emit finished();
}
