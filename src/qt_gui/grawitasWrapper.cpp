#include "grawitasWrapper.h"

// output
#include "output/graphGmlOutput.h"
#include "output/listCsvOutput.h"
#include "output/graphGraphmlOutput.h"
#include "output/graphGraphvizOutput.h"
#include "output/listCsvOutput.h"
#include "output/listHumanReadableOutput.h"
#include "output/listJsonOutput.h"



#include "helpers/stepTimer.h"
#include "parsing/coreTalkPageParsing.h"
#include "graphComputation/graphComputationCache.h"
#include "models/parsedTalkPage.h"


GrawitasWrapper::GrawitasWrapper(QObject *parent) : QObject(parent)
{

}

GrawitasWrapper::Format GrawitasWrapper::format_str_to_format(QString format_str)
{
    if(format_str == "Comment List (JSON)")
        return COMMENT_LIST_JSON;
    if(format_str == "Comment List (Human readable)")
        return COMMENT_LIST_HUMAN_READABLE;
    if(format_str == "Comment List (Csv)")
        return COMMENT_LIST_CSV;
    if(format_str == "User Network (GML)")
        return USER_NETWORK_GML;
    if(format_str == "User Network (GraphML)")
        return USER_NETWORK_GRAPHML;
    if(format_str == "User Network (GraphViz)")
        return USER_NETWORK_GRAPHVIZ;
    if(format_str == "Comment Network (GML)")
        return COMMENT_NETWORK_GML;
    if(format_str == "Comment Network (GraphML)")
        return COMMENT_NETWORK_GRAPHML;
    if(format_str == "Comment Network (GraphViz)")
        return COMMENT_NETWORK_GRAPHVIZ;
    if(format_str == "Two Mode Network (GML)")
        return TWO_MODE_NETWORK_GML;
    if(format_str == "Two Mode Network (GraphML)")
        return TWO_MODE_NETWORK_GRAPHML;
    if(format_str == "Two Mode Network (GraphViz)")
        return TWO_MODE_NETWORK_GRAPHVIZ;

    return COMMENT_LIST_JSON;
}


QString GrawitasWrapper::core(QString q_talk_page_syntax, Format format)
{

    using namespace Grawitas;

    std::string talk_page_syntax = q_talk_page_syntax.toStdString();

    ParsedTalkPage parsedTalkPage;

    parsedTalkPage = parseTalkPage(talk_page_syntax);

    // after splitting the talk page into comments compute their relationships based on the extracted indentations
    std::size_t curId = 1;
    for (auto& sec : parsedTalkPage) {
        calculateIds(sec.second, curId);
    }

    auto cache = GraphComputationCache(parsedTalkPage);

    std::stringstream ss;

    if(format == USER_NETWORK_GML)
        graphToGml(ss, cache.GetUserGraph());
    if(format == USER_NETWORK_GRAPHML)
        graphToGraphml(ss, cache.GetUserGraph());
    if(format == USER_NETWORK_GRAPHVIZ)
        graphToGraphviz(ss, cache.GetUserGraph());

    if(format == COMMENT_NETWORK_GML)
        graphToGml(ss, cache.GetCommentGraph());
    if(format == COMMENT_NETWORK_GRAPHML)
        graphToGraphml(ss, cache.GetCommentGraph());
    if(format == COMMENT_NETWORK_GRAPHVIZ)
        graphToGraphviz(ss, cache.GetCommentGraph());

    if(format == TWO_MODE_NETWORK_GML)
        graphToGml(ss, cache.GetTwoModeGraph());
    if(format == TWO_MODE_NETWORK_GRAPHML)
        graphToGraphml(ss, cache.GetTwoModeGraph());
    if(format == TWO_MODE_NETWORK_GRAPHVIZ)
        graphToGraphviz(ss, cache.GetTwoModeGraph());

    if(format == COMMENT_LIST_JSON)
        listToJson(ss, parsedTalkPage);
    if(format == COMMENT_LIST_CSV)
        listToCsv(ss, parsedTalkPage);
    if(format == COMMENT_LIST_HUMAN_READABLE)
        listToHumanReadable(ss, parsedTalkPage);

    return QString::fromStdString(ss.str());
}
