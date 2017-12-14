#include "coreWrapper.h"

#include "../../talkPageParser/parsing.h"
#include "../../talkPageParser/models.h"
#include "../../output/outputWrapper.h"

#include <boost/algorithm/string/trim.hpp>
#include <sstream>


CoreWrapper::CoreWrapper(QObject *parent)
: QObject(parent)
{}

QString CoreWrapper::core(QString q_talk_page_syntax, QString format_str)
{
    using namespace Grawitas;

    Format format = readable_to_format(format_str.toStdString());

    std::string talk_page_syntax = q_talk_page_syntax.toStdString();

    auto parsedTalkPage = parse_talk_page(talk_page_syntax);

    std::stringstream ss;

    output_in_format_to_stream(ss,format,parsedTalkPage,true,{"id", "parent_id", "user", "date", "section", "text"});

    return QString::fromStdString(ss.str());
}
