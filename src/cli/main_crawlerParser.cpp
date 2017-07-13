#include <iostream>
#include <fstream>
#include <QtCore>
#include <QMetaType>

#include <boost/program_options.hpp>

#include "output/outputWrapper.h"
#include "output/formats.h"
#include "parsing/coreTalkPageParsing.h"
#include "task.h"

Q_DECLARE_METATYPE(std::string)

namespace po = boost::program_options;

using namespace Grawitas;
using namespace std;

int main(int argc, char** argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
                    ("help,h", "Produces this help message.")

                    ("talk-page-list-file,i", po::value<string>(), "Path to an input file containing a talk page in the Wikipedia syntax.")
                    ("output-folder,o", po::value<string>(), "Path to an input file containing a talk page in the Wikipedia syntax.")

                    // network output
                    ("user-network-gml", po::bool_switch()->default_value(false), "Set this flag for exporting the user network in GML format.")
                    ("user-network-graphml", po::bool_switch()->default_value(false), "Set this flag for exporting the user network in GraphML format.")
                    ("user-network-graphviz", po::bool_switch()->default_value(false), "Set this flag for exporting the user network in GraphViz format.")

                    ("comment-network-gml", po::bool_switch()->default_value(false), "Set this flag for exporting the comment network in GML format.")
                    ("comment-network-graphml", po::bool_switch()->default_value(false), "Set this flag for exporting the comment network in GraphML format.")
                    ("comment-network-graphviz", po::bool_switch()->default_value(false), "Set this flag for exporting the comment network in GraphViz format.")

                    ("two-mode-network-gml", po::bool_switch()->default_value(false), "Set this flag for exporting the two-mode network in GML format.")
                    ("two-mode-network-graphml", po::bool_switch()->default_value(false), "Set this flag for exporting the two-mode network in GraphML format.")
                    ("two-mode-network-graphviz", po::bool_switch()->default_value(false), "Set this flag for exporting the two-mode network in GraphViz format.")

                    // list output
                    ("comment-list-csv", po::bool_switch()->default_value(false), "Set this flag for exporting the list of comments as a CSV format.")
                    ("comment-list-human-readable", po::bool_switch()->default_value(false), "Set this flag for exporting the list of comments as a human readable format.")
                    ("comment-list-json", po::bool_switch()->default_value(false), "Set this flag for exporting the list of comments as a JSON format (recommended).")

                    ("keep-raw-talk-pages", po::bool_switch()->default_value(false), "Set this flag if you want to keep the raw crawled talk pages.")
                    // misc
                    ("show-timings", po::bool_switch()->default_value(false), "Show the timings for the different steps.")
                    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // show help and exit program if --help is set
    if (vm.count("help")) {
        cout << desc << endl;
        return 0;
    }

    qRegisterMetaType<std::string>();

    QCoreApplication a(argc, argv);

    auto task = new Task(&a,vm);

    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();

    return 0;
}
