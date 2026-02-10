#pragma once

#include "PlaybackFrame.h"
#include "SignalBase.h"
#include "SynthConfiguration.h"

class FilterBase : public SignalBase
{
public:
	FilterBase(float gain, unsigned int numberOfChannels, unsigned int samplingRate);
	virtual ~FilterBase();

	virtual void GetSample(PlaybackFrame* frame, float absoluteTime) = 0;
	virtual bool HasOutput(float absoluteTime) const = 0;
	virtual void SetConfiguration(const SynthConfiguration* configuration) = 0;

public:

	unsigned int GetSamplingRate() const;
	unsigned int GetNumberOfChannels() const;
	float GetGain() const;

private:

	unsigned int _numberOfChannels;
	unsigned int _samplingRate;
	float _gain;
};

