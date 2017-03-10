import QtQuick 2.4
import QtQuick.Controls 1.4
import GrawitasWrapper 1.0

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
            width: 240
            height: 20
            model: [
                "Comment List (JSON)",
                "Comment List (Human readable)",
                "Comment List (Csv)",
                "User Network (GML)",
                "User Network (GraphML)",
                "User Network (GraphViz)",
                "Comment Network (GML)",
                "Comment Network (GraphML)",
                "Comment Network (GraphViz)",
                "Two Mode Network (GML)",
                "Two Mode Network (GraphML)",
                "Two Mode Network (GraphViz)"
            ]
            id: format_selector
        }

        Text {
            text: "Timing:"
            x: 350
            y: 14
            id: timing_label
        }
        Text {
            anchors.left: timing_label.right
            x: 400
            y: 14
        }

    }

    TextArea {
        x: 0
        y: 40
        width: 0.5*parent.width
        height: parent.height-40
        onTextChanged: function() {
            var format = grawitas_wrapper.format_str_to_format(format_selector.currentText);
            parsed_output.text = grawitas_wrapper.core(talk_page_syntax_input.text, format);
        }
        id: talk_page_syntax_input
    }
    TextArea {
        x: 0.5*parent.width
        y: 40
        width: 0.5*parent.width
        height: parent.height-40
        readOnly: true
        id: parsed_output
    }
}
