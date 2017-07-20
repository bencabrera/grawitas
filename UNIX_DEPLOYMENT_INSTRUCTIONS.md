# Unix Deployment Instructions

In order to provide binaries and make it easy to use them we provide of them in a standalone AppImage package containing all dependencies that should be need on a modern linux system.

To this end we use [linuxdeployqt](https://github.com/probonopd/linuxdeployqt). 

## Steps to do for deployment
1. Compile everything using qmake and make
2. Copy the binary folder to some other location (we will call it ```bin``` from now on)
3. Download the binary ```linuxdeployqt-continuous-x86_64.AppImage``` from the releases in the linuxdeployqt repository
4. For each of the cli programs simply run
```
./linuxdeployqt-continuous-x86_64.AppImage bin/cli/{grawitas_cli_name} -appimage
```
where ```{grawitas_cli_name}``` is the actual name of the cli binary. Then rename the created ```Application-x86_64.AppImage``` file to something more suitable.
5. For the GUI program run 
```
./linuxdeployqt-continuous-x86_64.AppImage bin/gui/grawitas_gui -appimage -qmldir={grawitas_folder}/src/gui/qml/
``` 
where ```{grawitas_folder}``` is the absolute path to the grawitas repository. Do this
**two times** before renaming the ```Application-x86_64.AppImage``` file to something more suitable.