TEMPLATE = subdirs

SUBDIRS = grawitas_core \
	  grawitas_gui \
	  grawitas_cli_core \
	  grawitas_cli_xml \

grawitas_core.file = core/grawitas_core.pro
grawitas_gui.file = gui/grawitas_gui.pro
grawitas_cli_core.file = cli/grawitas_cli_core.pro
grawitas_cli_xml.file = cli/grawitas_cli_xml.pro

grawitas_gui.depends = grawitas_core
grawitas_cli_core.depends = grawitas_core
grawitas_cli_xml.depends = grawitas_core
