#ifndef GRAWITASWRAPPER_H
#define GRAWITASWRAPPER_H

#include <QObject>
#include <QVariantList>

#include "output/formats.h"

class GrawitasWrapper : public QObject
{
    Q_OBJECT
public:

    explicit GrawitasWrapper(QObject *parent = 0);

    Q_INVOKABLE QString core(QString q_talk_page_syntax, QString format_str);

    Q_INVOKABLE void xml_dump_component(QString input_xml_path, QString output_folder, QVariantList formats);

signals:

public slots:
};

#endif // GRAWITASWRAPPER_H
