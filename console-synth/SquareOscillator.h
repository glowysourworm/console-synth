#pragma once
#include "AmplitudeOscillator.h"

class SquareOscillator : public AmplitudeOscillator
{
public:

	SquareOscillator(float frequency);
	~SquareOscillator() override;

	float GetSample(float absoluteTime) override;
};

