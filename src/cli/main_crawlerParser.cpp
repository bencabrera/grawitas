#include <iostream>
#include <fstream>
#include <QtCore>
#include <QMetaType>

#include "../../libs/cxxopts/include/cxxopts.hpp"

#include "output/outputWrapper.h"
#include "output/formats.h"
#include "parsing/coreTalkPageParsing.h"
#include "task.h"

Q_DECLARE_METATYPE(std::string)

using namespace Grawitas;
using namespace std;

int main(int argc, char** argv)
{
    cxxopts::Options options("grawitas_cli_crawler", "Grawitas CLI crawler parser.");
    options.add_options()
                    ("h,help", "Produces this help message.")

                    ("i,talk-page-list-file", "Path to an input file containing titles of Wikipedia articles.", cxxopts::value<string>())
                    ("o,output-folder", "Path to an input file containing a talk page in the Wikipedia syntax.", cxxopts::value<string>())

                    // network output
                    ("user-network-gml", "Set this flag for exporting the user network in GML format.")
                    ("user-network-graphml", "Set this flag for exporting the user network in GraphML format.")
                    ("user-network-graphviz", "Set this flag for exporting the user network in GraphViz format.")

                    ("comment-network-gml", "Set this flag for exporting the comment network in GML format.")
                    ("comment-network-graphml", "Set this flag for exporting the comment network in GraphML format.")
                    ("comment-network-graphviz", "Set this flag for exporting the comment network in GraphViz format.")

                    ("two-mode-network-gml", "Set this flag for exporting the two-mode network in GML format.")
                    ("two-mode-network-graphml", "Set this flag for exporting the two-mode network in GraphML format.")
                    ("two-mode-network-graphviz", "Set this flag for exporting the two-mode network in GraphViz format.")

                    // list output
                    ("comment-list-csv", "Set this flag for exporting the list of comments as a CSV format.")
                    ("comment-list-human-readable", "Set this flag for exporting the list of comments as a human readable format.")
                    ("comment-list-json", "Set this flag for exporting the list of comments as a JSON format (recommended).")

                    ("keep-raw-talk-pages", "Set this flag if you want to keep the raw crawled talk pages.")
                    // misc
                    ("show-timings", "Show the timings for the different steps.")
                    ;

	options.parse(argc, argv);

    // show help and exit program if --help is set
    if (options.count("help")) {
		cout << options.help() << endl;
        return 0;
    }

    qRegisterMetaType<std::string>();

    QCoreApplication a(argc, argv);

    auto task = new Task(&a,options);

    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();

    return 0;
}
