#pragma once

#include <QObject>
#include <QVariantList>

#include <set>
#include "../../output/formats.h"

class CrawlerWrapper : public QObject
{
    Q_OBJECT
	Q_PROPERTY(bool abort READ abort WRITE setAbort)
public:

    explicit CrawlerWrapper(QObject *parent = 0);
	~CrawlerWrapper();

    Q_INVOKABLE void crawl(QString input_file_path, QString output_folder, QVariantList readable_format_strs, bool keep_raw_talk_pages);
	void crawling(QString input_file_path, QString output_folder, QVariantList readable_format_strs, bool keep_raw_talk_pages);

    // QList<QObject*> root_objects;
	bool _abort;
	void setAbort(const bool &a) {
		_abort = a;
    }
    bool abort() const {
        return _abort;
    }

    QObject* _crawler_text_area;
};
