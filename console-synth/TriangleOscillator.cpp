#include "Constant.h"
#include "OscillatorBase.h"
#include "TriangleOscillator.h"
#include <cmath>

TriangleOscillator::TriangleOscillator(float frequency) : OscillatorBase(frequency)
{
}

TriangleOscillator::~TriangleOscillator()
{
}

float TriangleOscillator::GetMonoSample(float absoluteTime)
{
	float period = this->GetPeriod();
	float periodQuarter = 0.25f * period;
	float high = this->GetHigh();
	float low = this->GetLow();
	float sample = 0;

	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, period);

	// First Quadrant
	if (periodTime < periodQuarter)
	{
		sample = (2.0 * (high - low) / period) * periodTime;
	}

	// Second Quadrant
	else if (periodTime < (2.0 * periodQuarter))
	{
		sample = ((-2.0 * (high - low) / period) * (periodTime - (float)periodQuarter)) - low;
	}

	// Third Quadrant
	else if (periodTime < 3.0 * periodQuarter)
	{
		sample = (-2.0 * (high - low) / period) * (periodTime - (2.0 * periodQuarter));
	}

	// Fourth Quadrant
	else
	{
		sample = ((2.0 * (high - low) / period) * (periodTime - (3.0 * periodQuarter))) + low;
	}

	return sample;
}

OscillatorType TriangleOscillator::GetType() const
{
	return OscillatorType::Triangle;
}
