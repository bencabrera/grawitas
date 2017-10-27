#include "dumpParserThread.h"

#include "../../core/parsing/xmlDumpParserWrapper.h"
#include "../../core/output/outputHelpers.h"

DumpParserThread::DumpParserThread(QString _input_xml_path, QString _output_folder, std::set<Grawitas::Format> _formats)
    :input_xml_path(_input_xml_path.toStdString()),
      output_folder(_output_folder.toStdString()),
      formats(_formats)
{}

void DumpParserThread::run()
{
    std::string xml_path = input_xml_path;
    std::string output_path = normalize_folder_path(output_folder);
    Grawitas::xml_dump_parsing(xml_path, output_path, formats, { [this](std::string msg){ emit write_status(QString::fromStdString(msg)); } });
}
