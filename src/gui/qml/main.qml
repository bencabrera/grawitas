import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: qsTr("GraWiTas - A Grammar-based Wikpedia Talk Page Parser")

    MainForm {
        anchors.fill: parent
    }
}
