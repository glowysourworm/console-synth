#pragma once
#include "Constant.h"
#include "OscillatorBase.h"

class RandomOscillator : public OscillatorBase
{
public:

	RandomOscillator(float frequency, int numberOfDivisions);
	~RandomOscillator() override;

	float GetMonoSample(float absoluteTime) override;

	OscillatorType GetType() const override;

private:
	int _numberOfDivisions;
	float _periodDivisionTime;
	float* _randomValues;
};

