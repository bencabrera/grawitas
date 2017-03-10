import QtQuick 2.5
import QtQuick.Controls 1.4


Item {
    anchors.fill: parent

    TabView {
        id: tabView1
        anchors.fill: parent
        Tab {
            anchors.fill: parent
            title: "Core Parser Component"

            CoreComponentForm{
                anchors.fill: parent
            }
        }
        Tab {
            anchors.fill: parent
            title: "Crawler Component"

            Text {
                text: "Hello World2"
                anchors.fill: parent
            }
        }
        Tab {
            anchors.fill: parent
            title: "XML-Dump Component"

            Text {
                text: "Hello World3"
                anchors.fill: parent
            }
        }
    }


}
