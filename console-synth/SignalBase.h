#pragma once

#include "Constant.h"
#include "PlaybackFrame.h"

class SignalBase
{
public:
	SignalBase() 
	{
		_low = SIGNAL_LOW;
		_high = SIGNAL_HIGH;
	};
	SignalBase(float low, float high)
	{
		_low = low;
		_high = high;
	}
	virtual ~SignalBase() {}

	virtual void GetSample(PlaybackFrame* frame, float absoluteTime) = 0;

	float GetHigh() const { return _high; }
	float GetLow() const { return _low; }

private:

	float _low;
	float _high;
};
