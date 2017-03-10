import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: qsTr("GraWiTas - A grammar-based Wikpedia talk page parser")

    menuBar:MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Quit"
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: "View"
            MenuItem { text: "Core Parser Component" }
            MenuItem { text: "Crawler Component" }
            MenuItem { text: "XML-Dump Component" }
        }

        Menu {
            title: "Help"
            MenuItem { text: "About" }
        }
    }


    MainForm {
        anchors.fill: parent
    }
}
