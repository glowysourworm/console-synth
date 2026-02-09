#pragma once

#include "Constant.h"
#include "SignalChannelBase.h"
#include <exception>

class FilterChannelBase : public SignalChannelBase
{
public:
	FilterChannelBase(float gain, unsigned int samplingRate) : SignalChannelBase(SIGNAL_LOW, SIGNAL_HIGH)
	{
		_gain = gain;
		_samplingRate = samplingRate;
	}
	virtual ~FilterChannelBase() {}

	float GetSample(float absoluteTime) override 
	{
		throw new std::exception("SignalChannelBase::GetSample does not apply to FilterChannelBase. Please use FilterChannelBase::Apply");
	}
	virtual float Apply(float inputSample, float absoluteTime) = 0;
	virtual bool HasOutput(float absoluteTime) const = 0;

public:

	unsigned int GetSamplingRate() const { return _samplingRate; }
	float GetGain() const { return _gain; }

private:

	unsigned int _samplingRate;
	float _gain;
};
