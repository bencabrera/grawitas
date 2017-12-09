#pragma once

#include <QObject>
#include <QVariantList>

#include <set>
#include <set>

class CoreWrapper : public QObject
{
    Q_OBJECT
public:

    explicit CoreWrapper(QObject *parent = 0);

    Q_INVOKABLE QString core(QString q_talk_page_syntax, QString format_str);

    // QList<QObject*> root_objects;
};
