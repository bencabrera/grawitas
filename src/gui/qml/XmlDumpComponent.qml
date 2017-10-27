import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import GrawitasWrapper 1.0
import QtQuick.Dialogs 1.2
import "trim.js" as Trim;

Item {
	GridLayout {
		x: 20
		y: 20
		width: parent.width-40
		height: parent.height-40

		columns: 3


		Label {
			text: "Path to .xml file (containing a Wikipedia dump):"
		}
		TextField{
			Layout.fillWidth: true
			width: parent.width
			id: xml_folder_field
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
			onClicked: xml_file_dialog.open()

			FileDialog {
				id: xml_file_dialog
				title: "Please choose a .xml file (containing a Wikipedia dump)"
				folder: shortcuts.home
				visible: false

				onAccepted: {
					xml_folder_field.text = Trim.trim(fileUrl.toString());
				}
			}
		}

		Label {
			text: "Path to output folder:"
		}
		TextField{
			Layout.fillWidth: true
			width: parent.width
			id: output_folder_field
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
			onClicked: output_folder_dialog.open()


			FileDialog {
				id: output_folder_dialog
				title: "Please choose an output folder"
				folder: shortcuts.home
				selectFolder: true
				visible: false

				onAccepted: {
					output_folder_field.text = Trim.trim(folder.toString());
				}
			}
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
						id: comment_list_json_checkbox
					}
					CheckBox {
						text: "Comment List (Human readable)"
						checked: false
						id: comment_list_human_checkbox
					}
					CheckBox {
						text: "Comment List (Csv)"
						checked: false
						id: comment_list_csv_checkbox
					}
				}
				Column {
					spacing: 20

					CheckBox {
						text: "User Network (GML)"
						checked: false
						id: user_network_gml_checkbox
					}
					CheckBox {
						text: "User Network (GraphML)"
						checked: false
						id: user_network_graphml_checkbox
					}
					CheckBox {
						text: "User Network (GraphViz)"
						checked: false
						id: user_network_graphviz_checkbox
					}
				}
				Column {
					spacing: 20

					CheckBox {
						text: "Comment Network (GML)"
						checked: false
						id: comment_network_gml_checkbox
					}
					CheckBox {
						text: "Comment Network (GraphML)"
						checked: false
						id: comment_network_graphml_checkbox
					}
					CheckBox {
						text: "Comment Network (GraphViz)"
						checked: false
						id: comment_network_graphviz_checkbox
					}
				}
				Column {
					spacing: 20

					CheckBox {
						text: "Two Mode Network (GML)"
						checked: false
						id: two_mode_network_gml_checkbox
					}
					CheckBox {
						text: "Two Mode Network (GraphML)"
						checked: false
						id: two_mode_network_graphml_checkbox
					}
					CheckBox {
						text: "Two Mode Network (GraphViz)"
						checked: false
						id: two_mode_network_graphviz_checkbox
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
			objectName: "xml_status_text_area"
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
				var checkboxes = [
					comment_list_json_checkbox,
					comment_list_human_checkbox,
					comment_list_csv_checkbox,
					user_network_gml_checkbox,
					user_network_graphml_checkbox,
					user_network_graphviz_checkbox,
					comment_network_gml_checkbox,
					comment_network_graphml_checkbox,
					comment_network_graphviz_checkbox,
					two_mode_network_gml_checkbox,
					two_mode_network_graphml_checkbox,
					two_mode_network_graphviz_checkbox
				];

				var formats = [];
				for(var i = 0; i < checkboxes.length; i++)
				{
					if(checkboxes[i].checked)
					formats.push(checkboxes[i].text);
				}

				var xml_folder = xml_folder_field.text;
				var output_folder = output_folder_field.text;

				grawitas_wrapper.xml_dump_component(xml_folder, output_folder, formats);

			}
		}

	}

}
