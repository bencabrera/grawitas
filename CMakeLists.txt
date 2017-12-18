cmake_minimum_required(VERSION 3.1.0) 

project(Grawitas)

# set module path
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# set c++ standard to c++11
set (CMAKE_CXX_STANDARD 11)

# look for boost library (required)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


# look for xerces library
find_package(XercesC)
if(XercesC_FOUND)
	include_directories(${XercesC_INCLUDE_DIR})
endif()

# look for CURL library
find_package(CURL)
if(CURL_FOUND)
	include_directories(${CURL_INCLUDE_DIR})
endif()

# look for sqlite3
find_package(SQLite3)
if(SQLite3_FOUND)
	include_directories(${SQLite3_INCLUDE_DIR})
endif()

# look for QT
# find_package(Qt5Core)
# find_package(Qt5Widgets)
# find_package(Qt5Qml)
# find_package(Qt5Concurrent)
find_package(Qt5 COMPONENTS Core Widgets Qml Concurrent)

if(Qt5_FOUND)
	# Find includes in corresponding build directories
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	# Instruct CMake to run moc automatically when needed.
	set(CMAKE_AUTOMOC ON)
	# Instruct CMake to run rcc automatically when needed.
	set(CMAKE_AUTORCC ON)
endif()

# files
set(TALK_PAGE_PARSER_FILES
	src/talkPageParser/date.cpp
	src/talkPageParser/parsing.cpp
	src/talkPageParser/grammars/commentGrammar.cpp
	src/talkPageParser/grammars/userGrammar.cpp
	src/talkPageParser/grammars/dateGrammar.cpp
	src/talkPageParser/grammars/signatureGrammar.cpp
	src/talkPageParser/grammars/talkPageGrammar.cpp
	src/talkPageParser/grammars/commentEndingGrammar.cpp
	src/talkPageParser/grammars/sections/sectionGrammar.cpp
	src/talkPageParser/grammars/sections/outdentGrammar.cpp
	src/talkPageParser/grammars/preprocessing/htmlCleaningGrammar.cpp
)

set(OUTPUT_FILES
	src/output/commentSummary.cpp
	src/output/graphGmlOutput.cpp
	src/output/graphGraphmlOutput.cpp
	src/output/graphGraphvizOutput.cpp
	src/output/listCsvOutput.cpp
	src/output/listHumanReadableOutput.cpp
	src/output/listJsonOutput.cpp
	src/output/outputWrapper.cpp
	src/output/dateOutput.cpp
	src/output/formats.cpp
	src/output/outputHelpers.cpp
)

set(NETWORKS_FILES
	src/networks/graphComputationCache.cpp
)

set(XML_TO_SQL_FILES
	src/xmlToSql/xmlToSqliteHandler.cpp
	src/xmlToSql/getArticlesFromDb.cpp
	src/xmlToSql/getUsersFromDb.cpp
	src/xmlToSql/getFilteredCommentsFromDb.cpp
	src/xmlToSql/getIdsFromDb.cpp
)

set(HTTP_CRAWLER_FILES
	src/httpCrawler/crawling.cpp
	src/httpCrawler/getPagesFromWikipedia.cpp
)

set(MISC_FILES
	src/misc/stepTimer.cpp
	src/misc/readLinesFromFile.cpp
)

set(TEST_FILES
	src/tests/grammars/commentGrammarTests.cpp
	src/tests/grammars/dateGrammarTests.cpp
	src/tests/grammars/sectionGrammarTests.cpp
	src/tests/grammars/signatureGrammarTests.cpp
	src/tests/grammars/talkPageGrammarTests.cpp
	src/tests/grammars/userGrammarTests.cpp
)

add_library(grawitas_core_lib STATIC ${TALK_PAGE_PARSER_FILES} ${OUTPUT_FILES} ${NETWORKS_FILES} ${MISC_FILES})

add_executable(grawitas_cli_core src/cli/main_coreParser.cpp)
target_link_libraries (grawitas_cli_core grawitas_core_lib)

if(CURL_FOUND)
	add_executable(grawitas_cli_crawler src/cli/main_crawlerParser.cpp ${HTTP_CRAWLER_FILES})
	target_link_libraries(grawitas_cli_crawler grawitas_core_lib)
	target_link_libraries(grawitas_cli_crawler ${CURL_LIBRARY})
endif()

if(XercesC_FOUND AND SQLITE3_FOUND)
	add_executable(grawitas_cli_xml_to_db src/cli/main_xmlToDb.cpp ${XML_TO_SQL_FILES})
	target_link_libraries (grawitas_cli_xml_to_db grawitas_core_lib)
	target_link_libraries(grawitas_cli_xml_to_db ${XercesC_LIBRARY})
	target_link_libraries(grawitas_cli_xml_to_db ${SQLITE3_LIBRARIES})
endif()

if(SQLITE3_FOUND)
	add_executable(grawitas_cli_db_export src/cli/main_dbExport.cpp ${XML_TO_SQL_FILES})
	target_link_libraries (grawitas_cli_db_export grawitas_core_lib)
	target_link_libraries(grawitas_cli_db_export ${SQLITE3_LIBRARIES})
endif()

if(Qt5_FOUND)
	add_executable(grawitas_gui_core_parser src/gui/cpp/main_coreParserDemo.cpp src/gui/cpp/coreWrapper.cpp src/gui/qml/coreParserDemo.qrc)
	target_link_libraries(grawitas_gui_core_parser grawitas_core_lib)
	target_link_libraries(grawitas_gui_core_parser Qt5::Core)
	target_link_libraries(grawitas_gui_core_parser Qt5::Widgets)
	target_link_libraries(grawitas_gui_core_parser Qt5::Qml)
endif()

if(Qt5_FOUND AND CURL_FOUND)
	add_executable(grawitas_gui_crawler src/gui/cpp/main_crawler.cpp ${GUI_FILES} ${HTTP_CRAWLER_FILES} src/gui/cpp/crawlerWrapper.cpp src/gui/qml/crawler.qrc)
	target_link_libraries(grawitas_gui_crawler grawitas_core_lib)
	target_link_libraries(grawitas_gui_crawler Qt5::Core)
	target_link_libraries(grawitas_gui_crawler Qt5::Widgets)
	target_link_libraries(grawitas_gui_crawler Qt5::Concurrent)
	target_link_libraries(grawitas_gui_crawler Qt5::Qml)
	target_link_libraries(grawitas_gui_crawler ${CURL_LIBRARY})
endif()

add_executable(tests src/tests/main_tests.cpp ${TEST_FILES})
target_link_libraries(tests grawitas_core_lib)
