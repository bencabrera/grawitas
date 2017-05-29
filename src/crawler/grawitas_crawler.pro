TEMPLATE = lib

QT += quick
CONFIG += c++11

SOURCES += \
    talkpageFetcher.cpp \
    parsedTalkPageArchiver.cpp \
    crawlerThread.cpp \

HEADERS += \
    talkpageFetcher.h \
    parsedTalkPageArchiver.h \
    crawlerThread.h \

INCLUDEPATH += $$PWD/../core
