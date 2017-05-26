TEMPLATE = lib 

CONFIG += c++11

SOURCES += output/graphGmlOutput.cpp \
	output/graphGraphmlOutput.cpp \
	output/graphGraphvizOutput.cpp \
	output/listCsvOutput.cpp \
	output/listHumanReadableOutput.cpp \
	output/listJsonOutput.cpp \
	output/outputWrapper.cpp \
	helpers/stepTimer.cpp \
	output/dateOutput.cpp \
	output/formats.cpp \
	models/comment.cpp \
	models/parsedTalkPage.cpp \
	parsing/coreTalkPageParsing.cpp \
	graphComputation/graphComputationCache.cpp \
	parsing/grammars/commentGrammar.cpp \
	parsing/grammars/userGrammar.cpp \
	parsing/grammars/dateGrammar.cpp \
	parsing/grammars/signatureGrammar.cpp \
	parsing/grammars/talkPageGrammar.cpp \
	parsing/grammars/commentEndingGrammar.cpp \
	parsing/grammars/sectionGrammar.cpp \
	parsing/xmlDumpParsingHandler.cpp \
	parsing/xmlDumpParserWrapper.cpp \

LIBS += -lxerces-c
LIBS += -lboost_filesystem
LIBS += -lboost_system
