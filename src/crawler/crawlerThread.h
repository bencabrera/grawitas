#ifndef CRAWLERTHREAD_H
#define CRAWLERTHREAD_H

#include <QThread>
#include <set>
#include <string>
#include "output/formats.h"

enum CrawlerOptions {
    KEEP_TALK_PAGE_FILES
};

class CrawlerThread : public QThread
{
    Q_OBJECT
public:

    CrawlerThread(QString _input_file_path, QString _output_folder, std::set<Grawitas::Format> _formats, std::set<CrawlerOptions> _options);

    void run();

    QString input_file_path;
    QString output_folder;
    std::set<Grawitas::Format> formats;
    std::set<CrawlerOptions> options;

signals:
    void write_status(QString);
};

#endif // CRAWLERTHREAD_H
