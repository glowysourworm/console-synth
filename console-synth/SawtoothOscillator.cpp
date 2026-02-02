#include "AmplitudeOscillator.h"
#include "Constant.h"
#include "SawtoothOscillator.h"
#include <cmath>

SawtoothOscillator::SawtoothOscillator(float frequency) : AmplitudeOscillator(frequency, SIGNAL_LOW, SIGNAL_HIGH)
{
}

SawtoothOscillator::~SawtoothOscillator()
{
}

float SawtoothOscillator::GetSample(float absoluteTime)
{
	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, this->GetPeriod());

	return (((this->GetHigh() - this->GetLow()) / this->GetPeriod()) * periodTime) + this->GetLow();
}
