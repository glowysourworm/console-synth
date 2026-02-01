#pragma once

#include <chrono>

class StopWatch
{
private:
	using clock = std::chrono::time_point<std::chrono::system_clock>;

public:
	void mark();

	int markMilliseconds();

private:
	clock _last;
};
