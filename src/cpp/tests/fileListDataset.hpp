#pragma once

#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/filesystem.hpp>
#include <vector>

namespace fs = boost::filesystem;

namespace WikiTalkNet {
	namespace Tests {

		class FileListDataset {
			public:
				using sample = fs::path;
				enum { arity = 1 };

				using iterator = std::vector<boost::filesystem::path>::const_iterator;

				inline FileListDataset(const std::string pathToFolder, const std::string endsWith, const std::string extension)
				{
					fs::path folder(pathToFolder);
					if(fs::exists(folder) && fs::is_directory(folder))
					{
						for(auto it = fs::directory_iterator(folder); it != fs::directory_iterator(); it++)
						{
							std::string filenameWithoutEnding = (*it).path().stem().string();
							std::string fileExtension = (*it).path().extension().string();
							if (
									filenameWithoutEnding.length() >= endsWith.length() 
									&& 0 == filenameWithoutEnding.compare (filenameWithoutEnding.length() - endsWith.length(), endsWith.length(), endsWith) && fileExtension == extension
							   )
								_files.push_back(*it);
						}

						std::sort(_files.begin(), _files.end());
					}
				}

				inline iterator begin() const { return _files.cbegin(); }
				boost::unit_test::data::size_t size() const { return _files.size(); }

			private: 
				std::vector<fs::path> _files;
		};


	}
}


namespace boost { namespace unit_test { namespace data { namespace monomorphic {
	// registering as a proper dataset
	template <>
		struct is_dataset<WikiTalkNet::Tests::FileListDataset> : boost::mpl::true_ {};
}}}}
