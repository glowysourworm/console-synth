#pragma once

#include <chrono>
#include <ratio>

class StopWatch
{
private:

	// CPP Referernce: en.cppreference.com/w/cpp/chrono/duration.html
	//
	using clock = std::chrono::time_point<std::chrono::system_clock>;
	using milliMultiplier = std::ratio_multiply<std::ratio<1>, std::milli>;
	using microMultiplier = std::ratio_multiply<std::ratio<1>, std::micro>;
	using nanoMultiplier = std::ratio_multiply<std::ratio<1>, std::nano>;

public:

	/// <summary>
	/// Resets the stop watch
	/// </summary>
	void mark();

	/// <summary>
	/// Returns the current time rounded in terms of seconds
	/// </summary>
	double markSeconds();

	/// <summary>
	/// Returns the current time rounded in terms of milli-seconds
	/// </summary>
	double markMilli();

	/// <summary>
	/// Returns the current time rounded in terms of micro-seconds
	/// </summary>
	double markMicro();

	/// <summary>
	/// Returns the current time rounded in terms of nano-seconds
	/// </summary>
	double markNano();

	/// <summary>
	/// Returns the current time rounded in terms of seconds (does not update the last marker)
	/// </summary>
	double peek();

	/// <summary>
	/// Returns the current time rounded in terms of seconds (does not update the last marker)
	/// </summary>
	double peekMilli();

	/// <summary>
	/// Returns the current time rounded in terms of seconds (does not update the last marker)
	/// </summary>
	double peekMicro();

	/// <summary>
	/// Returns the current time rounded in terms of seconds (does not update the last marker)
	/// </summary>
	double peekNano();

private:

	std::chrono::duration<double> getUpdate();
	std::chrono::duration<double> markUpdate();

private:
	clock _last;
};