#pragma once

#include "FilterBase.h"
#include "kCathedral.h"

class AirwindowsEffect : public FilterBase
{
public:

	// Compression ratio is a value between [1, Infinity]
	AirwindowsEffect(float delaySeconds, float gain, int samplingRate );
	~AirwindowsEffect();

	float Apply(float sample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;

private:

	kCathedral* _effect;

	float _delay;

	float** _input;
	float** _output;
};

