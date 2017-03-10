import QtQuick 2.4
import QtQuick.Controls 1.4

Item {   
    Item {
        x: 0
        y: 0
        width: parent.width
        height: 40
        Text {
            text: "Format:"
            x: 10
            y: 14
        }
        ComboBox {
            x: 70
            y: 10
            width: 200
            height: 20
            model: [ "Banana", "Apple", "Coconut" ]
            id: format_selector
        }

        Text {
            text: "Timing:"
            x: 350
            y: 14
            id: timing_label
        }
        Text {
            text: "560ms"
            anchors.left: timing_label.right + 20
            x: 400
            y: 14
        }

    }

    TextArea {
        x: 0
        y: 40
        width: 0.5*parent.width
        height: parent.height - 50
        text:
            "Lorem ipsum dolor sit amet, consectetur adipisicing elit, " +
            "sed do eiusmod tempor incididunt ut labore et dolore magna " +
            "aliqua. Ut enim ad minim veniam, quis nostrud exercitation " +
            "ullamco laboris nisi ut aliquip ex ea commodo cosnsequat. ";
    }
    TextArea {
        x: 0.5*parent.width
        y: 40
        width: 0.5*parent.width
        height: parent.height - 50
        readOnly: true
        text:
            "Lorem ipsum dolor sit amet, consectetur adipisicing elit, " +
            "sed do eiusmod tempor incididunt ut labore et dolore magna " +
            "aliqua. Ut enim ad minim veniam, quis nostrud exercitation " +
            "ullamco laboris nisi ut aliquip ex ea commodo cosnsequat. ";
    }
}
