#include "dumpParserThread.h"

#include "../../core/parsing/xmlDumpParserWrapper.h"

DumpParserThread::DumpParserThread(QString _input_xml_path, QString _output_folder, std::set<Grawitas::Format> _formats)
    :input_xml_path(_input_xml_path),
      output_folder(_output_folder),
      formats(_formats)
{}

void DumpParserThread::run()
{
    std::string xml_path = input_xml_path.toStdString();
    std::string output_path = output_folder.toStdString();
    Grawitas::xml_dump_parsing(xml_path, output_path, formats, { [this](std::string msg){ emit write_status(QString::fromStdString(msg)); } });
}
