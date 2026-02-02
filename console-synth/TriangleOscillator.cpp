#include "AmplitudeOscillator.h"
#include "Constant.h"
#include "TriangleOscillator.h"
#include <cmath>

TriangleOscillator::TriangleOscillator(float frequency) : AmplitudeOscillator(frequency, SIGNAL_LOW, SIGNAL_HIGH)
{
}

TriangleOscillator::~TriangleOscillator()
{
}

float TriangleOscillator::GetSample(float absoluteTime)
{
	float period = this->GetPeriod();
	float periodQuarter = 0.25f * period;
	float high = this->GetHigh();
	float low = this->GetLow();

	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, period);

	// First Quadrant
	if (periodTime < periodQuarter)
	{
		return (2.0 * (high - low) / period) * periodTime;
	}

	// Second Quadrant
	else if (periodTime < (2.0 * periodQuarter))
	{
		return ((-2.0 * (high - low) / period) * (periodTime - (float)periodQuarter)) - low;
	}

	// Third Quadrant
	else if (periodTime < 3.0 * periodQuarter)
	{
		return (-2.0 * (high - low) / period) * (periodTime - (2.0 * periodQuarter));
	}

	// Fourth Quadrant
	else
	{
		return ((2.0 * (high - low) / period) * (periodTime - (3.0 * periodQuarter))) + low;
	}
}
