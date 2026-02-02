#include "AmplitudeOscillator.h"

AmplitudeOscillator::AmplitudeOscillator(float frequency, float low, float high)
{
	_frequency = frequency;
	_period = 1 / frequency;
	_low = low;
	_high = high;
}
AmplitudeOscillator::~AmplitudeOscillator()
{
}
float AmplitudeOscillator::GetFrequency() const
{
	return _frequency;
}
float AmplitudeOscillator::GetPeriod() const
{
	return _period;
}
float AmplitudeOscillator::GetLow() const
{
	return _low;
}
float AmplitudeOscillator::GetHigh() const
{
	return _high;
}