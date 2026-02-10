#include "LoopTimer.h"
#include "StopWatch.h"

LoopTimer::LoopTimer(double goalSeconds)
{
	_goalSeconds = goalSeconds;

	_secondTimer = new StopWatch();
	_milliTimer = new StopWatch();
	_microTimer = new StopWatch();
	_nanoTimer = new StopWatch();

	Reset();
}

LoopTimer::~LoopTimer()
{
	delete _secondTimer;
	delete _milliTimer;
	delete _microTimer;
	delete _nanoTimer;
}

void LoopTimer::Reset()
{
	// Resets counter to calculate next goal
	//
	_secondTimer->mark();
	_milliTimer->mark();
	_microTimer->mark();
	_nanoTimer->mark();

	_lastSeconds = 0;
	_lastMilli = 0;
	_lastMicro = 0;
	_lastNano = 0;
}

bool LoopTimer::Mark()
{
	double seconds = _secondTimer->markSeconds();
	double milli = _milliTimer->markMilli();
	double micro = _microTimer->markMicro();
	double nano = _nanoTimer->markNano();

	_avgSeconds += (seconds - _avgSeconds) / (_counterSec++ + 1);
	_avgMilli += (milli - _avgMilli) / (_counterMilli++ + 1);
	_avgMicro += (micro - _avgMicro) / (_counterMicro++ + 1);
	_avgNano += (nano - _avgNano) / (_counterNano++ + 1);

	_lastSeconds += seconds;
	_lastMilli += milli;
	_lastMicro += micro;
	_lastNano += nano;

	return this->GoalReached();
}

double LoopTimer::GetLastSeconds() const
{
	return _lastSeconds;
}

double LoopTimer::GetLastMilli() const
{
	return _lastMilli;
}

double LoopTimer::GetLastMicro() const
{
	return _lastMicro;
}

double LoopTimer::GetLastNano() const
{
	return _lastNano;
}

double LoopTimer::GetAvgSeconds() const
{
	return _avgSeconds;
}

double LoopTimer::GetAvgMilli() const
{
	return _avgMilli;
}

double LoopTimer::GetAvgMicro() const
{
	return _avgMicro;
}

double LoopTimer::GetAvgNano() const
{
	return _avgNano;
}

double LoopTimer::GetGoalSeconds() const
{
	return _goalSeconds;
}

bool LoopTimer::GoalReached() const
{
	return _lastSeconds > _goalSeconds;
}
