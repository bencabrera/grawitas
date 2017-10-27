function trim(path) {
	if(Qt.platform.os == "linux" || Qt.platform.os == "osx" || Qt.platform.os == "unix")
		return path.replace(/^(file:\/{2})/,"");

	if(Qt.platform.os == "windows")
		return path.replace(/^(file:\/{3})/,"");
}
