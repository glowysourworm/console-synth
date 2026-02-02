#include "AmplitudeOscillator.h"
#include "Constant.h"
#include "SineOscillator.h"
#include <cmath>

SineOscillator::SineOscillator(float frequency) : AmplitudeOscillator(frequency, SIGNAL_LOW, SIGNAL_HIGH)
{
}

SineOscillator::~SineOscillator()
{
}

float SineOscillator::GetSample(float absoluteTime)
{
	return sinf(2.0 * PI * this->GetFrequency() * absoluteTime);
}
