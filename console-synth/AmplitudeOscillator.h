#ifndef AMPLITUDEOSCILLATOR_H
#define AMPLITUDEOSCILLATOR_H

#include <math.h>
#include <random>

#include "Constant.h"

enum class AmplitudeOscillatorType
{
	Sine,
	Square,
	Random
};

class AmplitudeOscillator
{
public:
	AmplitudeOscillator(float frequency, AmplitudeOscillatorType type);
	~AmplitudeOscillator();

	float GetSample(float absoluteTime);

private:
	float _frequency;
	float _period;
	AmplitudeOscillatorType _type;

	float* _randomDivisions;
	float* _randomValues;
	int _randomLastDivisionIndex;
};

#endif