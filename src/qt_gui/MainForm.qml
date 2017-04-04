import QtQuick 2.5
import QtQuick.Controls 1.4


Item {
    anchors.fill: parent

    TabView {
        id: tabView1
        anchors.fill: parent
        anchors.margins: 10
        objectName: "tab_view"
        onCurrentIndexChanged: {
            grawitas_wrapper.tab_view_changed();
        }
        Tab {
            anchors.fill: parent
            title: "Crawler Component"

            CrawlerComponent{
                anchors.fill: parent
            }
        }
        Tab {
            anchors.fill: parent
            title: "XML-Dump Component"
            objectName: "dump_tab"

            XmlDumpComponent{
                anchors.fill: parent
                objectName: "dump_comp"
            }
        }
        Tab {
            anchors.fill: parent
            title: "Demo - Core Parser Component"

            CoreComponentForm{
                anchors.fill: parent
            }
        }
    }


}
