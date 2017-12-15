#include "crawlerWrapper.h"

#include "../../talkPageParser/models.h"
#include "../../output/outputWrapper.h"
#include "../../httpCrawler/crawling.h"
#include "../../misc/readLinesFromFile.h"

#include <QDateTime>
#include <iostream>
#include <QtConcurrent>
#include <boost/algorithm/string/trim.hpp>
#include <sstream>
#include <fstream>

namespace {
	std::set<Grawitas::Format> formats_from_variant_list(QVariantList readable_format_strs)
	{
		std::set<Grawitas::Format> formats;
		for(auto el : readable_format_strs)
		{
			if(!el.canConvert<QString>())
				return std::set<Grawitas::Format>();
			std::string readable_format_str = el.value<QString>().toStdString();
			formats.insert(Grawitas::readable_to_format(readable_format_str));
		}

		return formats;
	}
}


CrawlerWrapper::CrawlerWrapper(QObject *parent)
: QObject(parent),
_crawler_text_area(nullptr),
_abort(false)
{}

CrawlerWrapper::~CrawlerWrapper()
{
	_crawler_text_area = nullptr;
}

void CrawlerWrapper::crawl(QString input_file_path, QString output_folder, QVariantList readable_format_strs, bool keep_raw_talk_pages)
{
	QtConcurrent::run(this, &CrawlerWrapper::crawling, input_file_path, output_folder, readable_format_strs, keep_raw_talk_pages);
}

void CrawlerWrapper::crawling(QString input_file_path, QString output_folder, QVariantList readable_format_strs, bool keep_raw_talk_pages)
{
	try {
		auto formats = formats_from_variant_list(readable_format_strs);

		// TODO: error handling
		std::ifstream input_file(input_file_path.toStdString());
		auto titles = read_lines_from_file(input_file);

		Grawitas::AdditionalCrawlerOptions crawler_options;
		crawler_options.keep_raw_talk_pages = keep_raw_talk_pages;
		crawler_options.status_callback = [this](const std::string& msg) { 
			QString status_message = QString::fromStdString(msg);
			if(_crawler_text_area != nullptr)
			{
				auto current_time = QDateTime::currentDateTime();
				status_message = QString("[") + current_time.toString(QString("yyyy-MM-dd HH:mm:ss")) + QString("] ") + status_message;
				QMetaObject::invokeMethod(_crawler_text_area,"append",Qt::QueuedConnection,Q_ARG(QVariant, QVariant(status_message)));
			}
		};
		crawler_options.abort = &_abort;
		Grawitas::crawling(titles, output_folder.toStdString(), formats, crawler_options);
	}
	catch(const std::invalid_argument& e) {
		if(_crawler_text_area != nullptr)
		{
			QMetaObject::invokeMethod(_crawler_text_area,"append",Qt::QueuedConnection,Q_ARG(QVariant, QVariant(QString("ABORTING. An argument error appeared: "))));
			QMetaObject::invokeMethod(_crawler_text_area,"append",Qt::QueuedConnection,Q_ARG(QVariant, QVariant(QString(e.what()))));
		}
		return;
	}
	catch(const std::exception& e) {
		if(_crawler_text_area != nullptr)
		{
			QMetaObject::invokeMethod(_crawler_text_area,"append",Qt::QueuedConnection,Q_ARG(QVariant, QVariant(QString("ABORTING. The application terminated with an exception: "))));
			QMetaObject::invokeMethod(_crawler_text_area,"append",Qt::QueuedConnection,Q_ARG(QVariant, QVariant(QString(e.what()))));
		}
		return;
	}
}
