TEMPLATE = app

QT += qml quick
CONFIG += c++11

QMAKE_RPATHDIR = $$OUT_PWD/../core/ 
QMAKE_RPATHDIR += $$OUT_PWD/../crawler/

SOURCES += cpp/main.cpp \
    cpp/grawitasWrapper.cpp \
    cpp/dumpParserThread.cpp

HEADERS += \
    cpp/grawitasWrapper.h \
    cpp/dumpParserThread.h

RESOURCES += qml/qml.qrc

unix|win32: LIBS += -L$$OUT_PWD/../core/ -lgrawitas_core
unix|win32: LIBS += -L$$OUT_PWD/../crawler/ -lgrawitas_crawler

INCLUDEPATH += $$PWD/../core
INCLUDEPATH += $$PWD/../crawler

unix|win32: LIBS += -lxerces-c
unix|win32: LIBS += -lboost_filesystem
unix|win32: LIBS += -lboost_system
