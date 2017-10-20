#ifndef TASK_H
#define TASK_H

#include <QObject>
#include "../core/output/formats.h"
#include "../crawler/crawlerThread.h"
#include "../../libs/cxxopts/include/cxxopts.hpp"

class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent, const cxxopts::Options& options);
private:
	const cxxopts::Options& arguments;
	const std::set<Grawitas::Format> formats;
	const std::set<CrawlerOptions> options;

	static std::set<Grawitas::Format> formats_from_parameters(const cxxopts::Options& options);
	static std::set<CrawlerOptions> options_from_parameters(const cxxopts::Options& options);

public slots:
	void run();
	void print_status(std::string);

signals:
	void finished();
};

#endif // TASK_H
