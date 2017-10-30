#include <QGuiApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQmlContext>
#include <QMetaType>
#include "grawitasWrapper.h"

Q_DECLARE_METATYPE(std::string)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    GrawitasWrapper grawitas_wrapper;
    qmlRegisterType<GrawitasWrapper>("GrawitasWrapper", 1, 0, "GrawitasWrapper");
    qRegisterMetaType<std::string>();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("grawitas_wrapper",&grawitas_wrapper);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    grawitas_wrapper.root_objects = engine.rootObjects();
    grawitas_wrapper.tab_view_changed();

    return app.exec();
}
