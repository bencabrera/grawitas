#include <QGuiApplication>
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


    return app.exec();
}
