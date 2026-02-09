#pragma once
#include "OscillatorBase.h"
#include "PlaybackFrame.h"

class RandomOscillator : public OscillatorBase
{
public:

	RandomOscillator(float frequency, int numberOfDivisions);
	~RandomOscillator() override;

	float GetMonoSample(float absoluteTime) override;

private:
	int _numberOfDivisions;
	float _periodDivisionTime;
	float* _randomValues;
};

