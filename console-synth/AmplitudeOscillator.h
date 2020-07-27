#ifndef AMPLITUDEOSCILLATOR_H
#define AMPLITUDEOSCILLATOR_H

#include <math.h>

#include "Constant.h"

enum class AmplitudeOscillatorType
{
	Sine,
	Square
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
};

#endif