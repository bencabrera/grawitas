# General Information

## Abstract
Wikipedia offers researchers unique insights into the collaboration and communication patterns of a large self-regulating community of editors. The main medium of direct communication between editors of an article is the article's talk page. However, a talk page file is unstructured and therefore difficult to analyse automatically. 

Grawitas is a lightweight, fast parser for Wikipedia talk pages that takes the raw Wikipedia-syntax and output the structured content in various formats. In particular Grawitas is able to extract the relationship between comments, i.e. what comment is a reply for which other comment, as well as the meta data (username, date, indentation, etc.).

In addition to the core parser component we provide two ways to access the Wikipedia talk page data which then gets fed to the core parser. One the one hand we have a **crawler component** connecting to the Wikipedia.org website and extracting talk pages from the there. On the other hand we have a **xml-dump component** which runs on the Wikipedia XML-dumps that can be downloaded from [Wikipedia Dumps](https://en.wikipedia.org/wiki/Wikipedia:Database_download)

## Demo Video
To get an overview of Grawitas please watch the following demo video on YouTube (activate subtitles for brief descriptions).

[![Alt text](https://img.youtube.com/vi/ime-Pg32-0c/0.jpg)](https://www.youtube.com/watch?v=ime-Pg32-0c)

## Additional Reference and Citing the Article
The source code was made available as part of a publication in EACL 2017. If you are using the parser for your research please cite the article as

```Cabrera, B., Steinert, L., Ross, B. (2017). Grawitas: A Grammar-based Wikipedia Talk Page Parser. Proceedings of the Software Demonstrations of the 15th Conference of the European Chapter of the Association for Computational Linguistics, pp. 21-24.```

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
./grawitas_cli_crawler -i list_of_article_names.txt -o output_folder/ --comment-list-json --user-network-gml --keep-raw-talk-pages
```

# Getting the parser (to run)
To work with the parser you first have to obtain a running version of its binaries. This can be done by using one of two methods:

- Downloading the binaries for your operating system
- Compiling the source code for yourself

If you quickly want to use the parser as is for your project or have little experience in compiling from sources use the first method. If you want full control, want to modify the source, or have problems with the provided binaries for your system, then use the second method.

## Obtaining the binaries

The binaries can be found here. Currently we provide binaries for Linux (Ubuntu) and Windows.

To use them you just have to 

1. download the correct binaries for your operating system
2. install needed dependencies (see *Dependencies* down below)
2. open a command line (e.g. terminal for ubuntu, powershell for windows)
3. go to the directory where you saved the binaries,
4. run the binaries as described in *Usage* down below.

### Dependencies ###

*Ubuntu:*
* for binaries of the C++ components there should be no additional dependencies
* if you want to run the crawler component you need to install a Java JDK

*Windows:*
* for the C++ the *Visual C++ Redistributable* is needed. It can be found here and has to be installed.
* if you want to run the crawler component you need to install a Java JDK

## Compiling the source code yourself

The different parser components require different dependencies to be installed on the system. Details on how to install these dependecies are provided in the next operation system specific sections.

In general, the **core parser component** depends on different components of the c++ **boost library** as its only dependency. In more detail it depends on
- boost.spirit 
- boost.filesystem
- boost.system 
- boost.program_options
- boost.graph

The *xml-dump parser component* additionally depends on the **Xerces** c++ library for parsing XML-files.

Finally the crawler component is a Java program and thus needs some form of JDK installed to compile and run the source.

### Compiling under Linux (e.g. Ubuntu 16.04)
The following tutorial was testen on Ubuntu 16.04, but should also work similarly under newer or older Ubuntu versions and different distributions.

To compile the c++ do the following steps:

1. *Install the boost dependencies*. To do so type the following command in a console and follow the instructions:
	
	```
	sudo apt-get install libboost-all-dev
	```
	
	*Explanation:* 
	- ```sudo``` runs the next command as a super-user (that's why you have to type in your password)
	- ```apt-get install``` runs the apt package manager (the standard package manager under ubuntu) to install a package 
	- ```libboost-all-dev``` is the name of the package that contains all boost sources and precompiled headers

2. *Install the xerces dependencies.* Similar to step 1 run 

	```
	sudo apt-get install libxerces-c-dev
	```
	in a console to install the xerces development package.

3. *Install the cmake build tool and git revision control system* Finally we install the tool used for building the binaries.

	```
	sudo apt-get install cmake, git
	```

4. *Obtain the source code via git* and switch to the directory
	
	```
	git clone git@github.com:Ace7k3/grawitas.git
	cd grawitas
	```

5. *Build the source*. Now we can run cmake to generate the Makefiles to then compile the **GrawitasCoreParser** and the **GrawitasXmlDumpParser**.

	```
	mkdir bin 				# build a separate folder for binaries
	cd bin 					# switch to the new folder
	cmake ../src/cpp/ 		# run cmake to initialize Makefiles
	make 					# run make to compile 
	```

# Usage