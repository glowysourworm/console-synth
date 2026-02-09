#include "OscillatorBase.h"
#include "PlaybackFrame.h"
#include "SquareOscillator.h"
#include <cmath>

SquareOscillator::SquareOscillator(float frequency) : OscillatorBase(frequency)
{
}

SquareOscillator::~SquareOscillator()
{
}

float SquareOscillator::GetMonoSample(float absoluteTime)
{
	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, this->GetPeriod());
	float sample = 0;

	if (periodTime < this->GetPeriod() / 2.0)
		sample = this->GetHigh();

	else
		sample = this->GetLow();

	return sample;
}
