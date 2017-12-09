#include <QGuiApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQmlContext>
#include <QMetaType>
#include "crawlerWrapper.h"

Q_DECLARE_METATYPE(std::string)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

	CrawlerWrapper crawler_wrapper;
	qmlRegisterType<CrawlerWrapper>("CrawlerWrapper", 1, 0, "CrawlerWrapper");
    qRegisterMetaType<std::string>();

    QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("crawler_wrapper",&crawler_wrapper);
    engine.load(QUrl(QStringLiteral("qrc:/main_crawler.qml")));

	crawler_wrapper._crawler_text_area = engine.rootObjects().front()->findChild<QObject*>("crawler_status_text_area");

    return app.exec();
}
