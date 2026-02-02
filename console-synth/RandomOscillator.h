#pragma once
#include "AmplitudeOscillator.h"

class RandomOscillator : public AmplitudeOscillator
{
public:

	RandomOscillator(float frequency, float low, float high, int numberOfDivisions);
	~RandomOscillator() override;

	float GetSample(float absoluteTime) override;

private:
	int _numberOfDivisions;
	float _periodDivisionTime;
	float* _randomValues;
};

