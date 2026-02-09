#pragma once

#include "Constant.h"

class SignalChannelBase
{
public:
	SignalChannelBase()
	{
		_low = SIGNAL_LOW;
		_high = SIGNAL_HIGH;
	};
	SignalChannelBase(float low, float high)
	{
		_low = low;
		_high = high;
	}
	virtual ~SignalChannelBase() {}

	virtual float GetSample(float absoluteTime) = 0;

	float GetHigh() const { return _high; }
	float GetLow() const { return _low; }

private:

	float _low;
	float _high;
};
