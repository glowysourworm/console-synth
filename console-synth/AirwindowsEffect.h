#pragma once

#include "FilterBase.h"
#include "PlaybackFrame.h"
#include "SynthConfiguration.h"

class AirwindowsEffect : public FilterBase
{
public:

	// Compression ratio is a value between [1, Infinity]
	AirwindowsEffect(float delaySeconds, float gain, unsigned int numberOfChannels, unsigned int samplingRate);
	~AirwindowsEffect();

	virtual void GetSample(PlaybackFrame* frame, float absoluteTime) override;
	virtual bool HasOutput(float absoluteTime) const override;
	void SetConfiguration(const SynthConfiguration* configuration) override;

private:

	//kCathedral* _effect;

	float _delay;

	float** _input;
	float** _output;
};

