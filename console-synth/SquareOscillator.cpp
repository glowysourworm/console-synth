#include "AmplitudeOscillator.h"
#include "Constant.h"
#include "SquareOscillator.h"
#include <cmath>

SquareOscillator::SquareOscillator(float frequency) : AmplitudeOscillator(frequency, SIGNAL_LOW, SIGNAL_HIGH)
{
}

SquareOscillator::~SquareOscillator()
{
}

float SquareOscillator::GetSample(float absoluteTime)
{
	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, this->GetPeriod());

	if (periodTime < this->GetPeriod() / 2.0)
		return this->GetHigh();

	else
		return this->GetLow();
}
