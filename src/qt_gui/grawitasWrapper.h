#ifndef GRAWITASWRAPPER_H
#define GRAWITASWRAPPER_H

#include <QObject>

class GrawitasWrapper : public QObject
{
    Q_OBJECT
public:
    enum Format {
        USER_NETWORK_GML,
        USER_NETWORK_GRAPHML,
        USER_NETWORK_GRAPHVIZ,
        COMMENT_NETWORK_GML,
        COMMENT_NETWORK_GRAPHML,
        COMMENT_NETWORK_GRAPHVIZ,
        TWO_MODE_NETWORK_GML,
        TWO_MODE_NETWORK_GRAPHML,
        TWO_MODE_NETWORK_GRAPHVIZ,
        COMMENT_LIST_JSON,
        COMMENT_LIST_HUMAN_READABLE,
        COMMENT_LIST_CSV
    };
    Q_ENUMS(Format)

    explicit GrawitasWrapper(QObject *parent = 0);

    Q_INVOKABLE QString core(QString, Format format);
    Q_INVOKABLE Format format_str_to_format(QString format_str);

signals:

public slots:
};

#endif // GRAWITASWRAPPER_H
