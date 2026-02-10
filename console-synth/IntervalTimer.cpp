#include "IntervalTimer.h"
#include "StopWatch.h"

IntervalTimer::IntervalTimer()
{
	_stopWatch = new StopWatch();
	_stopWatch->mark();

	_lastSeconds = 0;
	_lastMilli = 0;
	_lastMicro = 0;
	_lastNano = 0;

	_avgSeconds = 0;
	_avgMilli = 0;
	_avgMicro = 0;
	_avgNano = 0;

	_counter = 0;
}

IntervalTimer::~IntervalTimer()
{
	delete _stopWatch;
}

void IntervalTimer::Mark()
{
    _lastSeconds = _stopWatch->peek();
	_lastMilli = _stopWatch->peekMilli();
	_lastMicro = _stopWatch->peekMicro();
	_lastNano = _stopWatch->peekNano();

	_stopWatch->mark();

	_avgSeconds += (_lastSeconds - _avgSeconds) / (_counter + 1);
	_avgMilli += (_lastMilli - _avgMilli) / (_counter + 1);
	_avgMicro += (_lastMicro - _avgMicro) / (_counter + 1);
	_avgNano += (_lastNano - _avgNano) / (_counter++ + 1);
}

void IntervalTimer::Reset()
{
	_stopWatch->mark();

	_lastSeconds = 0;
	_lastMilli = 0;
	_lastMicro = 0;
	_lastNano = 0;
}

void IntervalTimer::ClearAll()
{
	this->Reset();

	_avgSeconds = 0;
	_avgMilli = 0;
	_avgMicro = 0;
	_avgNano = 0;

	_counter = 0;
}

double IntervalTimer::LastSeconds() const
{
	return _lastSeconds;
}

double IntervalTimer::LastMilli() const
{
	return _lastMilli;
}

double IntervalTimer::LastMicro() const
{
	return _lastMicro;
}

double IntervalTimer::LastNano() const
{
	return _lastNano;
}

double IntervalTimer::AvgSeconds() const
{
	return _avgSeconds;
}

double IntervalTimer::AvgMilli() const
{
	return _avgMilli;
}

double IntervalTimer::AvgMicro() const
{
	return _avgMicro;
}

double IntervalTimer::AvgNano() const
{
	return _avgNano;
}
