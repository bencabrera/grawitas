TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    grawitasWrapper.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

unix|win32: LIBS += -L$$PWD/../../bin/release/ -lCoreLib

INCLUDEPATH += $$PWD/../cpp
DEPENDPATH += $$PWD/../cpp

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../bin/release/CoreLib.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../bin/release/libCoreLib.a

HEADERS += \
    grawitasWrapper.h

unix|win32: LIBS += -lxerces-c

unix|win32: LIBS += -lboost_filesystem

unix|win32: LIBS += -lboost_system
