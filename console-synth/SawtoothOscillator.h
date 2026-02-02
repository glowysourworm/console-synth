#pragma once
#include "AmplitudeOscillator.h"

class SawtoothOscillator : public AmplitudeOscillator
{
public:

	SawtoothOscillator(float frequency);
	~SawtoothOscillator() override;

	float GetSample(float absoluteTime) override;
};

