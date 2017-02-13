#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <chrono>
#include <sstream>

class StepTimer {

	public:
		void startTiming(const std::string label, const std::string description);
		void stopTiming(const std::string label);
		void printTimings(std::ostream& ostr);

	private:

		std::vector<std::pair<std::string, double>> finishedTimings;
		std::map<std::string, std::pair<std::string, std::chrono::time_point<std::chrono::steady_clock>>> timers;

		std::string timingToReadable(std::size_t milliseconds);

};
