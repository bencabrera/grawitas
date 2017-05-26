cmake_minimum_required(VERSION 2.8) 

project(Grawitas)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# look for boost library
find_package(Boost REQUIRED COMPONENTS program_options graph system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

# --- GrawitasCoreLib files ---
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

set(XML_DUMP_PARSING_FILES
	parsing/xmlDumpParsingHandler.cpp
	parsing/xmlDumpParserWrapper.cpp
)


add_library(GrawitasCoreLib STATIC ${OUTPUT_SOURCE_FILES} ${MODELS_SOURCE_FILES} ${PARSERS_SOURCE_FILES} ${GRAPH_COMPUTATION_SOURCE_FILES} ${GRAMMAR_SOURCE_FILES} ${XML_DUMP_PARSING_FILES})
target_link_libraries(GrawitasCoreLib ${Boost_LIBRARIES})

add_executable(GrawitasCliCoreParser main_coreParser.cpp)
target_link_libraries(GrawitasCliCoreParser ${Boost_LIBRARIES})
target_link_libraries(GrawitasCliCoreParser GrawitasCoreLib)

add_executable(GrawitasCliXmlDumpParser main_xmlDumpParser.cpp ${XML_DUMP_PARSING_FILES})
target_link_libraries(GrawitasCliXmlDumpParser ${Boost_LIBRARIES})
target_link_libraries(GrawitasCliXmlDumpParser xerces-c)
target_link_libraries(GrawitasCliXmlDumpParser GrawitasCoreLib)




# --- tests ---
set(TEST_SOURCE_FILES
	tests/helpers.cpp
	tests/grammars/commentGrammarTests.cpp
	tests/grammars/dateGrammarTests.cpp
	tests/grammars/sectionGrammarTests.cpp
	tests/grammars/signatureGrammarTests.cpp
	tests/grammars/talkPageGrammarTests.cpp
	tests/grammars/userGrammarTests.cpp
)

add_executable(tests tests/main_tests.cpp ${TEST_SOURCE_FILES})
target_link_libraries(tests ${Boost_LIBRARIES})
target_link_libraries(tests GrawitasCoreLib)
