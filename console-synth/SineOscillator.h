#pragma once
#include "AmplitudeOscillator.h"

class SineOscillator : public AmplitudeOscillator
{
public:

	SineOscillator(float frequency);
	~SineOscillator() override;

	float GetSample(float absoluteTime) override;
};

