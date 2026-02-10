#include "Constant.h"
#include "OscillatorBase.h"
#include "PlaybackFrame.h"
#include "SawtoothOscillator.h"
#include <cmath>

SawtoothOscillator::SawtoothOscillator(float frequency) : OscillatorBase(frequency)
{
}

SawtoothOscillator::~SawtoothOscillator()
{
}

float SawtoothOscillator::GetMonoSample(float absoluteTime)
{
	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, this->GetPeriod());

	return (((this->GetHigh() - this->GetLow()) / this->GetPeriod()) * periodTime) + this->GetLow();
}

OscillatorType SawtoothOscillator::GetType() const
{
	return OscillatorType::Sawtooth;
}
