#include "StopWatch.h"
#include <chrono>

void StopWatch::mark()
{
	_last = std::chrono::system_clock::now();
}
std::chrono::duration<double> StopWatch::getUpdate()
{
	clock end = std::chrono::system_clock::now();

	// Try the vendor -> stopwatch.hpp file to decipher this namespace.. or probably just read.
	// Not obvious how to get seconds; but this conversion gives you seconds - which has double
	// precision.
	//
	std::chrono::duration<double> elapsed_seconds = end - _last;

	// Apparently, this will return seconds, but as a duration, which has extreme accuracy from
	// the std library
	//
	return elapsed_seconds;
}
std::chrono::duration<double> StopWatch::markUpdate()
{
	clock end = std::chrono::system_clock::now();

	// Try the vendor -> stopwatch.hpp file to decipher this namespace.. or probably just read.
	// Not obvious how to get seconds; but this conversion gives you seconds - which has double
	// precision.
	//
	std::chrono::duration<double> elapsed_seconds = end - _last;

	_last += end - _last;

	// Apparently, this will return seconds, but as a duration, which has extreme accuracy from
	// the std library
	//
	return elapsed_seconds;
}
double StopWatch::markSeconds()
{
	// Updates our counter, and returns the duration since the previous udpate
	auto delta = this->markUpdate();

	// This should give the accurate count (SECONDS ONLY!)
	return delta.count();
}
double StopWatch::markMilli()
{
	// Updates our counter, and returns the duration since the previous udpate
	auto delta = this->markUpdate();

	// Create a duration with a multiplier to get milliseconds
	auto result = std::chrono::duration<double, milliMultiplier>(delta);

	// This should give the accurate count
	return result.count();
}
double StopWatch::markMicro()
{
	// Updates our counter, and returns the duration since the previous udpate
	auto delta = this->markUpdate();

	// Create a duration with a multiplier to get milliseconds
	auto result = std::chrono::duration<double, microMultiplier>(delta);

	// This should give the accurate count
	return result.count();
}
double StopWatch::markNano()
{
	// Updates our counter, and returns the duration since the previous udpate
	auto delta = this->markUpdate();

	// Create a duration with a multiplier to get milliseconds
	auto result = std::chrono::duration<double, nanoMultiplier>(delta);

	// This should give the accurate count
	return result.count();
}

double StopWatch::peek()
{
	// Gets an updated time delta in terms of seconds
	auto delta = this->getUpdate();

	return delta.count();
}

double StopWatch::peekMilli()
{
	// Gets an updated time delta in terms of seconds
	auto delta = this->getUpdate();

	// Create a duration with a multiplier to get milliseconds
	auto result = std::chrono::duration<double, milliMultiplier>(delta);

	return result.count();
}

double StopWatch::peekMicro()
{
	// Gets an updated time delta in terms of seconds
	auto delta = this->getUpdate();

	// Create a duration with a multiplier to get microseconds
	auto result = std::chrono::duration<double, microMultiplier>(delta);

	return result.count();
}

double StopWatch::peekNano()
{
	// Gets an updated time delta in terms of seconds
	auto delta = this->getUpdate();

	// Create a duration with a multiplier to get nanoseconds
	auto result = std::chrono::duration<double, nanoMultiplier>(delta);

	return result.count();
}
