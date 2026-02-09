#include "OscillatorBase.h"
#include "PlaybackFrame.h"
#include "RandomOscillator.h"
#include <cmath>
#include <cstdlib>
#include <exception>

RandomOscillator::RandomOscillator(float frequency, int numberOfDivisions) : OscillatorBase(frequency)
{
	if (numberOfDivisions <= 0)
		throw std::exception("Number of divisions of a RandomOscillator must be greater than zero");

	_numberOfDivisions = numberOfDivisions;
	_randomValues = new float[numberOfDivisions];

	// Size of a period division
	_periodDivisionTime = this->GetPeriod() / _numberOfDivisions;

	for (int index = 0; index < numberOfDivisions; index++)
	{
		// Random U[0,1] -> Low <-> High
		//
		_randomValues[index] = ((rand() / (double)RAND_MAX) * (this->GetHigh() - this->GetLow())) + this->GetLow();
	}
}

RandomOscillator::~RandomOscillator()
{
	delete[] _randomValues;
}

float RandomOscillator::GetMonoSample(float absoluteTime)
{
	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, this->GetPeriod());

	// Index into the random array
	int index = (int)std::floorf(periodTime / _periodDivisionTime);

	// Periodic value from the random array
	return _randomValues[index];
}
