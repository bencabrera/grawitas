#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <boost/program_options.hpp>

class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent, const boost::program_options::variables_map& new_vm);

private:
    const boost::program_options::variables_map& vm;

    public slots:
        void run();
        void print_status(std::string);

    signals:
        void finished();
};

#endif // TASK_H
