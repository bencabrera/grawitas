#include <QGuiApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQmlContext>
#include <QMetaType>
#include "coreWrapper.h"

Q_DECLARE_METATYPE(std::string)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

	CoreWrapper core_wrapper;
	qmlRegisterType<CoreWrapper>("CoreWrapper", 1, 0, "CoreWrapper");
    qRegisterMetaType<std::string>();

    QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("core_wrapper",&core_wrapper);
    engine.load(QUrl(QStringLiteral("qrc:/main_coreParserDemo.qml")));

    return app.exec();
}
