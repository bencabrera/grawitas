cmake_minimum_required(VERSION 3.1.0) 

project(Grawitas)

# look for boost library
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

# look for xerces library
find_package(XercesC REQUIRED)
include_directories( ${XercesC_INCLUDE_DIR} )

# look for QT
find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Widgets)
find_package(Qt5Qml)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run rcc automatically when needed.
set(CMAKE_AUTORCC ON)

# --- GrawitasCoreLib files ---
set(OUTPUT_SOURCE_FILES
	core/output/graphGmlOutput.cpp
	core/output/graphGraphmlOutput.cpp
	core/output/graphGraphvizOutput.cpp
	core/output/listCsvOutput.cpp
	core/output/listHumanReadableOutput.cpp
	core/output/listJsonOutput.cpp
	core/output/outputWrapper.cpp
	core/helpers/stepTimer.cpp
	core/output/dateOutput.cpp
	core/output/formats.cpp
)

set(MODELS_SOURCE_FILES
	core/models/comment.cpp
	core/models/parsedTalkPage.cpp
)

set(PARSERS_SOURCE_FILES
	core/parsing/coreTalkPageParsing.cpp
)

set(GRAPH_COMPUTATION_SOURCE_FILES
	core/graphComputation/graphComputationCache.cpp
)

set(GRAMMAR_SOURCE_FILES
	core/parsing/grammars/commentGrammar.cpp
	core/parsing/grammars/userGrammar.cpp
	core/parsing/grammars/dateGrammar.cpp
	core/parsing/grammars/signatureGrammar.cpp
	core/parsing/grammars/talkPageGrammar.cpp
	core/parsing/grammars/commentEndingGrammar.cpp
	core/parsing/grammars/sections/sectionGrammar.cpp
	core/parsing/grammars/sections/outdentGrammar.cpp
	core/parsing/grammars/preprocessing/htmlCleaningGrammar.cpp
)

set(XML_DUMP_PARSING_FILES
	core/parsing/xmlDumpParsingHandler.cpp
	core/parsing/xmlDumpParserWrapper.cpp
)

set(CRAWLER_FILES
	crawler/crawlerThread.cpp
	crawler/parsedTalkPageArchiver.cpp
	crawler/talkpageFetcher.cpp
)

set(GUI_FILES
	gui/cpp/dumpParserThread.cpp
	gui/cpp/grawitasWrapper.cpp
)


add_library(grawitas_core_lib STATIC ${OUTPUT_SOURCE_FILES} ${MODELS_SOURCE_FILES} ${PARSERS_SOURCE_FILES} ${GRAPH_COMPUTATION_SOURCE_FILES} ${GRAMMAR_SOURCE_FILES} ${XML_DUMP_PARSING_FILES})

add_executable(grawitas_cli_core cli/main_coreParser.cpp)
target_link_libraries(grawitas_cli_core grawitas_core_lib)

add_executable(grawitas_cli_xml cli/main_xmlDumpParser.cpp)
target_link_libraries(grawitas_cli_xml grawitas_core_lib)
target_link_libraries (grawitas_cli_xml ${XercesC_LIBRARY})

add_executable(grawitas_cli_crawler cli/main_crawlerParser.cpp cli/task.cpp ${CRAWLER_FILES})
target_link_libraries(grawitas_cli_crawler grawitas_core_lib)
target_link_libraries(grawitas_cli_crawler Qt5::Core)
target_link_libraries(grawitas_cli_crawler Qt5::Network)

add_executable(grawitas_gui gui/cpp/main.cpp ${GUI_FILES} ${CRAWLER_FILES} gui/qml/qml.qrc)
target_link_libraries(grawitas_gui grawitas_core_lib)
target_link_libraries(grawitas_gui Qt5::Core)
target_link_libraries(grawitas_gui Qt5::Network)
target_link_libraries(grawitas_gui Qt5::Widgets)
target_link_libraries(grawitas_gui Qt5::Qml)
target_link_libraries(grawitas_gui ${XercesC_LIBRARY})
