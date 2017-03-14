#include <QGuiApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "grawitasWrapper.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    GrawitasWrapper grawitas_wrapper;
    qmlRegisterType<GrawitasWrapper>("GrawitasWrapper", 1, 0, "GrawitasWrapper");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("grawitas_wrapper",&grawitas_wrapper);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject* crawler_text_area = engine.rootObjects().front()->findChild<QObject*>("crawler_status_text_area");
    QObject* xml_dump_text_area = engine.rootObjects().front()->findChild<QObject*>("xml_dump_status_text_area");
    grawitas_wrapper._crawler_text_area = crawler_text_area;
    grawitas_wrapper._xml_dump_text_area = xml_dump_text_area;

    return app.exec();
}
