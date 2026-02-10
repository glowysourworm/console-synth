#pragma once

#include "StopWatch.h"

class PlaybackClock
{
public:

	PlaybackClock();
	~PlaybackClock();

	/// <summary>
	/// Starts tracking the time
	/// </summary>
	void Start();

	/// <summary>
	/// Saves current time
	/// </summary>
	void Mark();

	/// <summary>
	/// Clears and re-starts the timer
	/// </summary>
	void Reset();

	/// <summary>
	/// Returns current time in seconds
	/// </summary>
	double GetTime();

	/// <summary>
	/// Returns the last mark time
	/// </summary>
	double GetMarkTime();

	/// <summary>
	/// Returns time since last mark
	/// </summary>
	double GetDelta();

private:

	double _startTime;
	double _currentTime;
	
	StopWatch* _stopWatch;
};

