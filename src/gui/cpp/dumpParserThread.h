#ifndef DUMPPARSERTHREAD_H
#define DUMPPARSERTHREAD_H

#include <QObject>
#include <QThread>

#include <set>
#include <string>
#include "../../core/output/formats.h"

class DumpParserThread : public QThread
{
    Q_OBJECT
public:
    DumpParserThread(QString _input_file_path, QString _output_folder, std::set<Grawitas::Format> _formats);

    void run();

    QString input_xml_path;
    QString output_folder;
    std::set<Grawitas::Format> formats;

signals:
    void write_status(QString);
};

#endif // DUMPPARSERTHREAD_H
