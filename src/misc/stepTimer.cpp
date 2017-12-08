#include "stepTimer.h"
#include <algorithm>

void StepTimer::startTiming(const std::string label, const std::string description)
{
	if(timers.find(label) != timers.end())
		throw std::logic_error("Tried to start timer with existing label again");

	timers.insert({ label, { description, std::chrono::steady_clock::now() } });
}

void StepTimer::stopTiming(const std::string label)
{
	auto it = timers.find(label);
	if(it == timers.end())
		throw std::logic_error("Tried to stop non-started timer.");

	auto endTime = std::chrono::steady_clock::now();
	auto diff = static_cast<std::size_t>(std::chrono::duration<double, std::milli>(endTime-it->second.second).count());
	finishedTimings.push_back({ it->second.first, diff });

	timers.erase(it);
}

void StepTimer::printTimings(std::ostream& ostr)
{
	std::size_t maxStrLength = 0;
	for(auto timing : finishedTimings)
		maxStrLength = std::max(maxStrLength, timing.first.length());

	for(auto timing : finishedTimings)
		ostr << std::left << std::setw(maxStrLength+3) << timing.first + ": " << timingToReadable(timing.second) << std::endl;
}

std::string StepTimer::timingToReadable(std::size_t milliseconds)
{
	std::stringstream ss;

	auto hours = (milliseconds / (1000*60*60));
	auto mins =  (milliseconds % (1000*60*60)) / (1000*60);
	auto seconds =  ((milliseconds % (1000*60*60)) % (1000*60)) / 1000;
	auto milli =  (((milliseconds % (1000*60*60)) % (1000*60)) % 1000);
	ss << hours << "h " << std::setw(2) << mins << "m " << std::setw(2) << seconds << "s " << std::setw(4) << milli << "ms";

	return ss.str();
}
