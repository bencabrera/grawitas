#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <boost/program_options.hpp>
#include "output/formats.h"
#include "crawlerThread.h"

class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent, const boost::program_options::variables_map& vm);
private:
	const boost::program_options::variables_map& vm;
	const std::set<Grawitas::Format> formats;
	const std::set<CrawlerOptions> options;

	static std::set<Grawitas::Format> formats_from_parameters(const boost::program_options::variables_map& vm);
	static std::set<CrawlerOptions> options_from_parameters(const boost::program_options::variables_map& vm);

public slots:
	void run();
	void print_status(std::string);

signals:
	void finished();
};

#endif // TASK_H
