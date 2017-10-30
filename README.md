# General Information

## Abstract
Wikipedia offers researchers unique insights into the collaboration and communication patterns of a large self-regulating community of editors. The main medium of direct communication between editors of an article is the article's talk page. However, a talk page file is unstructured and therefore difficult to analyse automatically. 

Grawitas is a lightweight, fast parser for Wikipedia talk pages that takes the raw Wikipedia-syntax and outputs the structured content in various formats. In particular Grawitas is able to extract the relationship between comments, i.e. what comment is a reply for which other comment, as well as the meta data (username, date, indentation, etc.).

In addition to the core parser component we provide two ways to access the Wikipedia talk page data which then gets fed to the core parser. One the one hand we have a *crawler component* connecting to the Wikipedia.org website and extracting talk pages from the there. On the other hand we have a *xml-dump component* which runs on the Wikipedia XML-dumps that can be downloaded from [Wikipedia Dumps](https://en.wikipedia.org/wiki/Wikipedia:Database_download)

Since Grawitas was developed in cross-platform C++ it can be run on **Linux** and **Windows**. In principle other Unix variants should work as well. 

## Demo Video
To get an overview of the Grawitas GUI please watch the following demo video on YouTube (activate subtitles for brief descriptions). Note that there have been slight changes to the program between the making of the video and the publishing of the current version.

[![Alt text](https://img.youtube.com/vi/ime-Pg32-0c/0.jpg)](https://www.youtube.com/watch?v=ime-Pg32-0c)

## Additional Reference and Citing the Article
The source code was made available as part of a publication in EACL 2017. If you are using the parser for your research please cite the article as

```Cabrera, B., Steinert, L., Ross, B. (2017). Grawitas: A Grammar-based Wikipedia Talk Page Parser. Proceedings of the Software Demonstrations of the 15th Conference of the European Chapter of the Association for Computational Linguistics, pp. 21-24.```

## Feedback, Issues and Bug reports
For feedback or questions please contact *benjamin.cabrera[at]uni-due.de*. If you find any bugs or have feature requests please use the github issue tracker on this repository.

# Installation  - i.e. getting the parser (to run)

To work with Grawitas you first have to obtain a running version of its binaries. This can be done by using one of two methods: downloading already compiled binaries or compiling the source code for yourself.

The compilation process can be a bit tricky on Windows. As a result, we provide packaged binaries for Windows that should work out of the box. On Linux, however, packaging is harder but because the compilation process is relatively easy you can compile Grawitas yourself. To this end we describe the compilation process on an exemplary Ubuntu 16.04 machine. 

## Windows - Using the compiled binaries

The packaged binaries for Windows can be found if you go to [the release tab of the repository](https://github.com/Ace7k3/grawitas/releases). 

The are two types of releases: *CLI only* and *CLI and GUI*. The former is much smaller and contains only the programs which can be run through the command-line interface. If you want the graphical user interface as well download the CLI and GUI latter release.

After downloading a release unzip the packed folder. In it you will find the mentioned .exe - binaries. To start the GUI double-click the grawitas_gui.exe. The CLI programs can be used by starting a terminal, switching to the extracted folder and starting the programs from there.

For more information on how to use the programs see the Usage section.

## Linux - Compiling the source code yourself

The different parser components require different dependencies to be installed on the system. 

In general, the *core parser component* mainly depends on the [boost.spirit](http://www.boost.org/) parser framework, as well as on [boost.graph](http://www.boost.org/). The *xml-dump parser component* additionally depends on the [*Xerces-C*](https://xerces.apache.org/) library for parsing XML-files. The crawler component uses components of QT5::Network. The GUI, containing all functionalities of the above, depends on all the libraries.

### Exemplary compilation process (for a fresh Ubuntu 16.04 install)

In the following section we will go through the compilation process assuming you want to compile all programs. 

1. *Install the dependencies through packages*. First we will have to install the mentioned dependencies. On Ubuntu 16.04 this can easily be done by installing some packages:

- `libboost-all-dev` - Boost packages, including the needed boost.spirit and boost.graph
- `libxerces-c-dev` - Xerces XML parser that is used in the XML parsing parts of the program
- `cmake` - CMake build tool used for generating the Makefiles
- `qtdeclarative5-dev`, `qml-module-qtquick-controls`, `qml-module-qtquick-layouts`, `qml-module-qtquick-dialogs` - QT 5 dependencies

	In total to install all these packages simply run
	```
	sudo apt-get install libboost-all-dev libxerces-c-dev cmake qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-layouts qml-module-qtquick-dialogs
	```
	
	*Explanation:* 
	- ```sudo``` runs the next command as a super-user (that's why you have to type in your password)
	- ```apt-get install``` runs the apt package manager (the standard package manager under ubuntu) to install a package 

2. *Obtain the source code*. In principle there are different ways of obtaining the source code from the repository. However, note that the program depends on other projects included as submodules. Thus those have to be obtained as well. It is probably easiest to create an github account, set up ssh keys, cloning the repository via ssh and then pulling the submodules.
	
	```
	git clone git@github.com:bencabrera/grawitas.git
	cd grawitas
	git submodule update --init --recursive
	```

3. *Run CMake configuration process*. Now we can run cmake to generate the Makefiles. To this end we first create a build folder `bin`. So inside the grawitas root directory run e.g.:

	```
	mkdir bin
	cd bin 
	cmake -DCMAKE_RELEASE_TYPE=Release ../src/
	```


4. *Build the source*. Now we can run make to compile everything

	```
	make
	```

5. You should now see 4 binaries in the `bin` folder which you can run (consider the next section for a description of the functionality).


# Usage
The full Grawitas toolset consists of four binaries. Three of those are cli (command line interface) programs and one is the GUI (graphical user interface). For the cli tools there is always a ```--help``` argument that can be passed to the program that leads to displaying a help message which should be mostly self-explanatory.
- **grawitas_cli_core** provides only the basic parsing functionality. It can be fed a raw talk page in [WikiMarkup](https://en.wikipedia.org/wiki/Help:Wiki_markup) and outputs the parsed talk page structure in various output formats (see Formats). 
Exemplary command:
```
./grawitas_cli_core --input-talk-page-file talk_page_file.wikimd --comment-list-json comment_list_file.json --user-network-gml user_network_file.gml
```

- **grawitas_cli_crawler** provides a crawler on top of the core parsing functionality. The user can specify a list of articles for which the talk page should be downloaded from en.wikipedia.org, parsed, and then stored in a file using various formats. 
Exemplary command:
```
./grawitas_cli_crawler -i list_of_article_names.txt -o output_folder/ --comment-list-json --user-network-gml --keep-raw-talk-pages
```

- **grawitas_cli_xml** works on [full xml dumps of wikipedia](https://en.wikipedia.org/wiki/Wikipedia:Database_download). This allows for a large scale analysis of all talk pages in Wikipedia.
Exemplary command:
```
./grawitas_cli_xml --input-xml-folder folder_with_dump_files/ --output-folder output_folder/ --comment-list-json
```

- **grawitas_gui** is the GUI containing mostly the same features as the cli programs. For a quick overview have a look at the video above.
Start the GUI from the main folder running the following command from a terminal.
```
./grawitas_gui
```



