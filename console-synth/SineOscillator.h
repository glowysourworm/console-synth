#pragma once
#include "OscillatorBase.h"
#include "PlaybackFrame.h"

class SineOscillator : public OscillatorBase
{
public:

	SineOscillator(float frequency);
	~SineOscillator() override;

	float GetMonoSample(float absoluteTime) override;
};

