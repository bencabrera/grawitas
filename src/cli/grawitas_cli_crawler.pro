TEMPLATE = app

QT += quick
CONFIG += c++11

SOURCES += main_crawlerParser.cpp

unix|win32: LIBS += -L$$OUT_PWD/../core/ -lgrawitas_core

INCLUDEPATH += $$PWD/../core
INCLUDEPATH += $$PWD/../crawler

unix|win32: LIBS += -lboost_filesystem
unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_program_options
