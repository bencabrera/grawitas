TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += cpp/main.cpp \
    cpp/grawitasWrapper.cpp \
    cpp/talkpageFetcher.cpp \
    cpp/parsedTalkPageArchiver.cpp \
    cpp/crawlerThread.cpp \
    cpp/dumpParserThread.cpp

HEADERS += \
    cpp/grawitasWrapper.h \
    cpp/talkpageFetcher.h \
    cpp/parsedTalkPageArchiver.h \
    cpp/crawlerThread.h \
    cpp/dumpParserThread.h

RESOURCES += qml/qml.qrc

unix|win32: LIBS += -L$$OUT_PWD/../core/ -lgrawitas_core

INCLUDEPATH += $$PWD/../core

unix|win32: LIBS += -lxerces-c
unix|win32: LIBS += -lboost_filesystem
unix|win32: LIBS += -lboost_system
