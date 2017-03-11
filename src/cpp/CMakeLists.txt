cmake_minimum_required(VERSION 2.8) 

project(WikipediaParsers)
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost REQUIRED COMPONENTS program_options graph system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

set(OUTPUT_SOURCE_FILES
	output/graphGmlOutput.cpp
	output/graphGraphmlOutput.cpp
	output/graphGraphvizOutput.cpp
	output/listCsvOutput.cpp
	output/listHumanReadableOutput.cpp
	output/listJsonOutput.cpp
	output/outputWrapper.cpp
	helpers/stepTimer.cpp
	output/dateOutput.cpp
	output/formats.cpp
)


set(MODELS_SOURCE_FILES
	models/comment.cpp
	models/parsedTalkPage.cpp
)

set(PARSERS_SOURCE_FILES
	parsing/coreTalkPageParsing.cpp
)

set(GRAPH_COMPUTATION_SOURCE_FILES
	graphComputation/graphComputationCache.cpp
)

set(GRAMMAR_SOURCE_FILES
	parsing/grammars/commentGrammar.cpp
	parsing/grammars/userGrammar.cpp
	parsing/grammars/dateGrammar.cpp
	parsing/grammars/signatureGrammar.cpp
	parsing/grammars/talkPageGrammar.cpp
	parsing/grammars/commentEndingGrammar.cpp
	parsing/grammars/sectionGrammar.cpp
)


set(TEST_SOURCE_FILES
	tests/optimalTextMapping.cpp
	tests/jsonToCommentList.cpp
	tests/helpers.cpp
)

set(XML_DUMP_PARSING_FILES
	parsing/xmlDumpParsingHandler.cpp
)


add_library(CoreLib STATIC ${OUTPUT_SOURCE_FILES} ${MODELS_SOURCE_FILES} ${PARSERS_SOURCE_FILES} ${GRAPH_COMPUTATION_SOURCE_FILES} ${GRAMMAR_SOURCE_FILES})

add_executable(GrawitasCoreParser main_coreParser.cpp)
target_link_libraries(GrawitasCoreParser ${Boost_LIBRARIES})
target_link_libraries(GrawitasCoreParser CoreLib)

add_executable(GrawitasXmlDumpParser main_xmlDumpParser.cpp ${XML_DUMP_PARSING_FILES})
target_link_libraries(GrawitasXmlDumpParser ${Boost_LIBRARIES})
target_link_libraries(GrawitasXmlDumpParser xerces-c)
target_link_libraries(GrawitasXmlDumpParser CoreLib)
target_link_libraries(GrawitasXmlDumpParser pthread)

add_executable(tests tests/main_tests.cpp ${GRAMMAR_SOURCE_FILES} ${TEST_SOURCE_FILES})
target_link_libraries(tests ${Boost_LIBRARIES})
target_link_libraries(tests MainLibrary)
