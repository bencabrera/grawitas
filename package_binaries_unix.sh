#!/bin/bash

BINARY_FOLDER=$1
PACKAGE_FOLDER=$2

if [ ! -d ${PACKAGE_FOLDER} ]; then
	mkdir -p ${PACKAGE_FOLDER};
fi

if [ ! -d ${PACKAGE_FOLDER}/libs ]; then
	mkdir -p ${PACKAGE_FOLDER}/libs;
fi

if [ ! -d ${PACKAGE_FOLDER}/bin ]; then
	mkdir -p ${PACKAGE_FOLDER}/bin;
fi

cp ${BINARY_FOLDER}/cli/grawitas_cli_core ${PACKAGE_FOLDER}/bin/
cp ${BINARY_FOLDER}/cli/grawitas_cli_crawler ${PACKAGE_FOLDER}/bin/
cp ${BINARY_FOLDER}/cli/grawitas_cli_xml ${PACKAGE_FOLDER}/bin/

cp ${BINARY_FOLDER}/core/libgrawitas_core.so.1 ${PACKAGE_FOLDER}/libs/
cp ${BINARY_FOLDER}/crawler/libgrawitas_crawler.so.1 ${PACKAGE_FOLDER}/libs/

cp ${BINARY_FOLDER}/gui/grawitas_gui ${PACKAGE_FOLDER}/bin/

cp  binary_wrapper_dummy.txt ${PACKAGE_FOLDER}/.wrapper
cd ${PACKAGE_FOLDER}
ln -s .wrapper grawitas_cli_core
chmod +x grawitas_cli_core
cd ..

