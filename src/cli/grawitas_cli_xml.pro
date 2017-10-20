TEMPLATE = app

CONFIG += c++11

QMAKE_RPATHDIR += $$OUT_PWD/../core/ 

SOURCES += main_xmlDumpParser.cpp

unix|win32: LIBS += -L$$OUT_PWD/../core/ -lgrawitas_core

INCLUDEPATH += $$PWD/../core

unix|win32: LIBS += -lxerces-c
