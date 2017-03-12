import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import GrawitasWrapper 1.0


Item {
    GridLayout {
        x: 20
        y: 20
        width: parent.width-40
        height: parent.height-40

        columns: 3


        Label {
            text: "Path to file containing Wikipedia URLs:"
        }
        TextField{
            Layout.fillWidth: true
            width: parent.width
        }
        Button{
            Label{
                text: "..."
                width: 20
                height: 20
                anchors.centerIn: parent
            }
            width: 20
            height: 20
        }

        Label {
            text: "Path to folder for output files:"
        }
        TextField{
            Layout.fillWidth: true
            width: parent.width
        }
        Button{
            Label{
                text: "..."
                width: 20
                height: 20
                anchors.centerIn: parent
            }
            width: 20
            height: 20
        }

        GroupBox {
            title: "Output Formats"
            Layout.columnSpan: 3
            width: parent.width
            Layout.fillWidth: true

            Row {
                spacing: 40
                width: parent.width
                Column {
                    spacing: 20

                    CheckBox {
                        text: "Comment List (JSON)"
                        checked: false
                    }
                    CheckBox {
                        text: "Comment List (Human readable)"
                        checked: false
                    }
                    CheckBox {
                        text: "Comment List (Csv)"
                        checked: false
                    }
                }
                Column {
                    spacing: 20

                    CheckBox {
                        text: "User Network (GML)"
                        checked: false
                    }
                    CheckBox {
                        text: "User Network (GraphML)"
                        checked: false
                    }
                    CheckBox {
                        text: "User Network (GraphViz)"
                        checked: false
                    }
                }
                Column {
                    spacing: 20

                    CheckBox {
                        text: "Comment Network (GML)"
                        checked: false
                    }
                    CheckBox {
                        text: "Comment Network (GraphML)"
                        checked: false
                    }
                    CheckBox {
                        text: "Comment Network (GraphViz)"
                        checked: false
                    }
                }
                Column {
                    spacing: 20

                    CheckBox {
                        text: "Two Mode Network (GML)"
                        checked: false
                    }
                    CheckBox {
                        text: "Two Mode Network (GraphML)"
                        checked: false
                    }
                    CheckBox {
                        text: "Two Mode Network (GraphViz)"
                        checked: false
                    }
                }
            }
        }

        Label {
            text: "Status"
            Layout.columnSpan: 3
        }

        TextArea{
            Layout.columnSpan: 3
            Layout.fillHeight: true
            Layout.fillWidth: true
            width: parent.width
            height: parent.height
            readOnly: true
        }

        Button{
            Layout.columnSpan: 3
            Layout.fillWidth: true
            width: parent.width
            Label {
                text: "Run"
                anchors.centerIn: parent
            }
            onClicked: {
                //grawitas_wrapper.request();
                grawitas_wrapper.request2();
            }
        }

    }

}
