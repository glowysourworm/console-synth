#include "PlaybackClock.h"
#include "StopWatch.h"

PlaybackClock::PlaybackClock()
{
	_stopWatch = new StopWatch();
	_startTime = 0;
	_currentTime = 0;
}

PlaybackClock::~PlaybackClock()
{
	delete _stopWatch;
}

void PlaybackClock::Start()
{
	_stopWatch->mark();
}

void PlaybackClock::Mark()
{
	_currentTime += _stopWatch->markSeconds();
}

void PlaybackClock::Reset()
{
	_startTime = 0;
	_currentTime = 0;

	this->Start();
}

double PlaybackClock::GetTime()
{
	return _currentTime + _stopWatch->peek();
}

double PlaybackClock::GetMarkTime()
{
	return _currentTime;
}

double PlaybackClock::GetDelta()
{
	return _stopWatch->peek();
}
