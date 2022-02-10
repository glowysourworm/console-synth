#include "AmplitudeOscillator.h"

AmplitudeOscillator::AmplitudeOscillator(float frequency, AmplitudeOscillatorType type)
{
	_frequency = frequency;
	_period = 1.0 / frequency;
	_type = type;
	
	_randomDivisions = new float[4] { 0.25, 0.5, 0.75, 1 };
	_randomValues = new float[4]{ 0.1, 1, 0.6, 0.3 };
	_randomLastDivisionIndex = 0;
}

AmplitudeOscillator::~AmplitudeOscillator()
{
	delete[] _randomDivisions;
	delete[] _randomValues;
}

float AmplitudeOscillator::GetSample(float absoluteTime)
{
	switch (_type)
	{
		case AmplitudeOscillatorType::Sine:
		default:
			return 0.5 + 0.5 * sin(2.0 * PI * _frequency * absoluteTime);

		case AmplitudeOscillatorType::Square:
		{
			if (fmodf(absoluteTime, _period) > (_period / 2.0))
				return 0;
			else
				return 1;
		}
		
		case AmplitudeOscillatorType::Random:
		{
			float periodRatio = fmodf(absoluteTime, _period);
			int currentDivisionIndex = 0;

			// Cycle through the period divisions
			for (int i = 0; i < 4; i++)
			{
				if (periodRatio < _randomDivisions[i])
				{
					currentDivisionIndex = i;
					break;
				}
			}

			// Advance the sample
			if (currentDivisionIndex != _randomLastDivisionIndex)
				_randomLastDivisionIndex = currentDivisionIndex;

			return _randomValues[_randomLastDivisionIndex];
		}
	}
}