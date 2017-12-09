import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: qsTr("GraWiTas - Core Parser Demo")

	Item {   
    	anchors.fill: parent
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
		}

		TextArea {
			x: 0
			y: 40
			width: 0.5*parent.width
			height: parent.height-40
			onTextChanged: function() {
				var format_str = format_selector.currentText;
				if(format_str.length > 0)
				parsed_output.text = core_wrapper.core(talk_page_syntax_input.text, format_str);
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
}
