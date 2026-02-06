#pragma once

#include "StopWatch.h"

class LoopTimer
{
public:

	LoopTimer(double goalSeconds);
	~LoopTimer();

	/// <summary>
	/// Resets the timers, and last goal times. The averages are kept as left by the
	/// previous Mark() call. 
	/// </summary>
	void Reset();

	/// <summary>
	/// Updates stream, calculates running averages based on this call. Returns true if the
	/// goal has been reached.
	/// </summary>
	bool Mark();

	double GetLastSeconds() const;
	double GetLastMilli() const;
	double GetLastMicro() const;
	double GetLastNano() const;

	double GetAvgSeconds() const;
	double GetAvgMilli() const;
	double GetAvgMicro() const;
	double GetAvgNano() const;

	double GetGoalSeconds() const;
	bool GoalReached() const;

private:

	double _goalSeconds;

	double _lastSeconds;
	double _lastMilli;
	double _lastMicro;
	double _lastNano;

	double _avgSeconds;
	double _avgMilli;
	double _avgMicro;
	double _avgNano;

	unsigned long _counterSec;
	unsigned long _counterMilli;
	unsigned long _counterMicro;
	unsigned long _counterNano;

	StopWatch* _secondTimer;
	StopWatch* _milliTimer;
	StopWatch* _microTimer;
	StopWatch* _nanoTimer;
};

