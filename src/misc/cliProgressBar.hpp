#include <iostream>
#include <iomanip>
#include <sstream>

namespace Shared {

	inline void cli_progress_bar(std::size_t i_progress, std::size_t n, std::size_t bar_width = 90)
	{
		double ratio = static_cast<double>(i_progress) / n;
		double percentage = 100 * ratio;

		std::cout << "[";
		std::size_t pos = bar_width * ratio;
		for (std::size_t i = 0; i < bar_width; ++i) {
			if (i < pos) std::cout << "=";
			else if (i == pos) std::cout << ">";
			else std::cout << " ";
		}
		std::cout << "] ";

		std::stringstream ss;

		ss << std::setw(5) << std::fixed << std::setprecision(1) << percentage << " %";
		std::cout << ss.str();
		std::cout << "\r"; // set carriage return
		std::cout.flush();
	}

}
