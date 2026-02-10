#pragma once

#include "StopWatch.h"

class IntervalTimer
{
public:

	IntervalTimer();
	~IntervalTimer();

	/// <summary>
	/// Takes the time at the current moment, and stores accumulators.
	/// </summary>
	void Mark();

	/// <summary>
	/// Resets all accumulators and starts the stopwatch at the current moment.
	/// </summary>
	void Reset();

	/// <summary>
	/// Performs a reset and clears all accumulators variables.
	/// </summary>
	void ClearAll();

public:

	double LastSeconds() const;
	double LastMilli() const;
	double LastMicro() const;
	double LastNano() const;

	double AvgSeconds() const;
	double AvgMilli() const;
	double AvgMicro() const;
	double AvgNano() const;

private:

	StopWatch* _stopWatch;

	double _lastSeconds;
	double _lastMilli;
	double _lastMicro;
	double _lastNano;

	double _avgSeconds;
	double _avgMilli;
	double _avgMicro;
	double _avgNano;

	int _counter;
};

