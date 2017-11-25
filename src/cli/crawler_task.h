#pragma once

#include <string>
#include <set>
#include <vector>

#include "../crawler/crawling.h"
#include <QObject>

class CrawlerTask : public QObject
{
    Q_OBJECT
public:
	CrawlerTask(QObject *parent, const std::vector<std::string>& titles, const std::string& output_folder, const std::set<Grawitas::Format>& formats);

private:

	const std::vector<std::string>& _titles;
	std::string _output_folder;
	const std::set<Grawitas::Format>& _formats;
	// const std::set<CrawlerOptions> options;

public slots:
	void run();
signals:
	void finished();
};
