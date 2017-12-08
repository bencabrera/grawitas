cmake_minimum_required(VERSION 3.1.0) 

project(Grawitas)

# set c++ standard to c++11
set (CMAKE_CXX_STANDARD 11)

# look for boost library
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

# look for xerces library
find_package(XercesC REQUIRED)
include_directories( ${XercesC_INCLUDE_DIR} )

# look for CURL library
find_package(CURL)
include_directories(${CURL_INCLUDE_DIR})

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

# files
set(TALK_PAGE_PARSER_FILES
	talkPageParser/parsing.cpp
	talkPageParser/grammars/commentGrammar.cpp
	talkPageParser/grammars/userGrammar.cpp
	talkPageParser/grammars/dateGrammar.cpp
	talkPageParser/grammars/signatureGrammar.cpp
	talkPageParser/grammars/talkPageGrammar.cpp
	talkPageParser/grammars/commentEndingGrammar.cpp
	talkPageParser/grammars/sections/sectionGrammar.cpp
	talkPageParser/grammars/sections/outdentGrammar.cpp
	talkPageParser/grammars/preprocessing/htmlCleaningGrammar.cpp
)

set(OUTPUT_FILES
	output/commentSummary.cpp
	output/graphGmlOutput.cpp
	output/graphGraphmlOutput.cpp
	output/graphGraphvizOutput.cpp
	output/listCsvOutput.cpp
	output/listHumanReadableOutput.cpp
	output/listJsonOutput.cpp
	output/outputWrapper.cpp
	output/dateOutput.cpp
	output/formats.cpp
	output/outputHelpers.cpp
)

set(NETWORKS_FILES
	networks/graphComputationCache.cpp
)

set(XML_DUMP_CRAWLER_FILES
	xmlDumpCrawler/xmlDumpParsingHandler.cpp
	xmlDumpCrawler/xmlDumpParsing.cpp
)

set(HTTP_CRAWLER_FILES
	httpCrawler/crawling.cpp
	httpCrawler/getPagesFromWikipedia.cpp
)

set(GUI_FILES
	gui/cpp/dumpParserThread.cpp
	gui/cpp/grawitasWrapper.cpp
)

set(MISC_FILES
	misc/stepTimer.cpp
)

set(TEST_FILES
	tests/grammars/commentGrammarTests.cpp
	tests/grammars/dateGrammarTests.cpp
	tests/grammars/sectionGrammarTests.cpp
	tests/grammars/signatureGrammarTests.cpp
	tests/grammars/talkPageGrammarTests.cpp
	tests/grammars/userGrammarTests.cpp
	tests/helpers.cpp
)

add_library(grawitas_core_lib STATIC ${TALK_PAGE_PARSER_FILES} ${OUTPUT_FILES} ${NETWORKS_FILES} ${MISC_FILES})

add_executable(grawitas_cli_core cli/main_coreParser.cpp)
target_link_libraries (grawitas_cli_core grawitas_core_lib)

add_executable(grawitas_cli_xml cli/main_xmlDumpParser.cpp ${XML_DUMP_CRAWLER_FILES})
target_link_libraries (grawitas_cli_xml grawitas_core_lib)
target_link_libraries (grawitas_cli_xml ${XercesC_LIBRARY})

add_executable(grawitas_cli_crawler cli/main_crawlerParser.cpp ${HTTP_CRAWLER_FILES})
target_link_libraries(grawitas_cli_crawler grawitas_core_lib)
target_link_libraries(grawitas_cli_crawler ${CURL_LIBRARY})

# add_executable(network_test cli/main_networkTest.cpp)
# target_link_libraries(network_test Qt5::Core)
# target_link_libraries(network_test Qt5::Network)

# add_executable(grawitas_gui gui/cpp/main.cpp ${GUI_FILES} ${CRAWLER_FILES} gui/qml/qml.qrc)
# target_link_libraries(grawitas_gui grawitas_core_lib)
# target_link_libraries(grawitas_gui Qt5::Core)
# target_link_libraries(grawitas_gui Qt5::Network)
# target_link_libraries(grawitas_gui Qt5::Widgets)
# target_link_libraries(grawitas_gui Qt5::Qml)
# target_link_libraries(grawitas_gui ${XercesC_LIBRARY})

# add_executable(core_tests core/tests/main_tests.cpp ${TEST_FILES})
