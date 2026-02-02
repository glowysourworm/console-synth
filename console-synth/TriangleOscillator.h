#pragma once
#include "AmplitudeOscillator.h"

class TriangleOscillator : public AmplitudeOscillator
{
public:

	TriangleOscillator(float frequency);
	~TriangleOscillator() override;

	float GetSample(float absoluteTime) override;
};

