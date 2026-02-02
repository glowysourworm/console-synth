#include "AmplitudeOscillator.h"
#include "RandomOscillator.h"
#include <cmath>
#include <cstdlib>
#include <exception>

RandomOscillator::RandomOscillator(float frequency, float low, float high, int numberOfDivisions) : AmplitudeOscillator(frequency, low, high)
{
	if (numberOfDivisions <= 0)
		throw std::exception("Number of divisions of a RandomOscillator must be greater than zero");

	// Size of a period division
	_periodDivisionTime = this->GetPeriod() / _numberOfDivisions;

	_numberOfDivisions = numberOfDivisions;
	_randomValues = new float[numberOfDivisions];

	for (int index = 0; index < numberOfDivisions; index++)
	{
		// Random U[0,1] -> Low <-> High
		//
		_randomValues[index] = ((rand() / (double)RAND_MAX) * (high - low)) + low;
	}
}

RandomOscillator::~RandomOscillator()
{
	delete[] _randomValues;
}

float RandomOscillator::GetSample(float absoluteTime)
{
	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, this->GetPeriod());

	// Index into the random array
	int index = (int)fmodf(periodTime, _periodDivisionTime);

	return _randomValues[index];
}
