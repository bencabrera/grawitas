cmake_minimum_required(VERSION 2.8) 

project(WikipediaParsers)
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(BOOST_ROOT /usr/local/include/boost/)
set(BOOST_LIBRARYDIR /usr/local/lib/)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS program_options regex graph system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

set(OUTPUT_SOURCE_FILES
	src/output/graphGmlOutput.h
	src/output/graphGmlOutput.cpp
	src/output/graphGraphmlOutput.h
	src/output/graphGraphmlOutput.cpp
	src/output/graphGraphvizOutput.h
	src/output/graphGraphvizOutput.cpp
	src/output/listCsvOutput.h
	src/output/listCsvOutput.cpp
	src/output/listHumanReadableOutput.h
	src/output/listHumanReadableOutput.cpp
	src/output/listJsonOutput.h
	src/output/listJsonOutput.cpp
	src/output/outputWrapper.h
	src/output/outputWrapper.cpp
	src/output/stepTimer.h
	src/output/stepTimer.cpp
	src/output/dateOutput.h
	src/output/dateOutput.cpp
	)


set(MODELS_SOURCE_FILES
	src/models/comment.cpp
	src/models/comment.h
	src/models/graph.h
	src/models/parsedTalkPage.h
	src/models/parsedTalkPage.cpp
	)

set(PARSERS_SOURCE_FILES
	#src/parsers/cleaningSyntax.h
	#src/parsers/cleaningSyntax.cpp
	src/parsers/commentParser.h
	src/parsers/commentParser.cpp
	#src/parsers/talkPageParsing.h
	#src/parsers/talkPageParsing.cpp
	#src/parsers/regexCombinatoricalParser.h
	#src/parsers/regexCombinatoricalParser.cpp
	src/parsers/talkPageParser.cpp
	src/parsers/talkPageParser.h
	)

set(GRAPH_COMPUTATION_SOURCE_FILES
	src/graphComputation/graphComputationCache.cpp
	src/graphComputation/graphComputationCache.h
	)

set(GRAMMAR_SOURCE_FILES
	src/parsers/grammars/commentGrammar.cpp
	src/parsers/grammars/userGrammar.cpp
	src/parsers/grammars/dateGrammar.cpp
	src/parsers/grammars/signatureGrammar.cpp
	src/parsers/grammars/talkPageGrammar.cpp
	src/parsers/grammars/commentEndingGrammar.cpp
	src/parsers/grammars/sectionGrammar.cpp
	)


set(TEST_SOURCE_FILES
	src/tests/optimalTextMapping.cpp
	src/tests/optimalTextMapping.h
	src/tests/jsonToCommentList.cpp
	src/tests/jsonToCommentList.h
	src/tests/helpers.h
	src/tests/helpers.cpp
	)

set(XPATH_SOURCE_FILES
	libs/tinyxpath/action_store.cpp
	libs/tinyxpath/htmlutil.cpp
	libs/tinyxpath/lex_util.cpp
	libs/tinyxpath/node_set.cpp
	libs/tinyxpath/tinystr.cpp
	libs/tinyxpath/tinyxml.cpp
	libs/tinyxpath/tinyxmlparser.cpp
	libs/tinyxpath/tinyxmlerror.cpp
	libs/tinyxpath/tokenlist.cpp
	libs/tinyxpath/xpath_expression.cpp
	libs/tinyxpath/xml_util.cpp
	libs/tinyxpath/xpath_processor.cpp
	libs/tinyxpath/xpath_stack.cpp
	libs/tinyxpath/xpath_static.cpp
	libs/tinyxpath/xpath_stream.cpp
	libs/tinyxpath/xpath_syntax.cpp)

ADD_LIBRARY( MainLibrary STATIC ${OUTPUT_SOURCE_FILES} ${MODELS_SOURCE_FILES} ${PARSERS_SOURCE_FILES} ${GRAPH_COMPUTATION_SOURCE_FILES} )
ADD_LIBRARY( TinyXPath STATIC ${XPATH_SOURCE_FILES} )

add_executable(WikiTalkPageParser src/main_talkPageParser.cpp ${GRAMMAR_SOURCE_FILES})
target_link_libraries(WikiTalkPageParser MainLibrary)
target_link_libraries(WikiTalkPageParser ${Boost_LIBRARIES})

add_executable(WikiRevisionHistoryParser src/main_revisionHistoryParser.cpp)
target_link_libraries(WikiRevisionHistoryParser MainLibrary)
target_link_libraries(WikiRevisionHistoryParser TinyXPath)
target_link_libraries(WikiRevisionHistoryParser ${Boost_LIBRARIES})

add_executable(talkNetTests src/main_tests.cpp ${GRAMMAR_SOURCE_FILES} ${TEST_SOURCE_FILES})
target_link_libraries(talkNetTests MainLibrary)
target_link_libraries(talkNetTests ${Boost_LIBRARIES})

add_executable(spiritTest src/main_spiritTest.cpp)
target_link_libraries(spiritTest MainLibrary)
target_link_libraries(spiritTest ${Boost_LIBRARIES})
