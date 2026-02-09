#include "OscillatorBase.h"
#include "PlaybackFrame.h"
#include "SineOscillator.h"
#include <cmath>
#include <numbers>

SineOscillator::SineOscillator(float frequency) : OscillatorBase(frequency)
{
}

SineOscillator::~SineOscillator()
{
}

float SineOscillator::GetMonoSample(float absoluteTime)
{
	return sinf(2.0 * std::numbers::pi * this->GetFrequency() * absoluteTime);
}
