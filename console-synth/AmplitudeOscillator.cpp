#include "AmplitudeOscillator.h"

AmplitudeOscillator::AmplitudeOscillator(float frequency, AmplitudeOscillatorType type)
{
	_frequency = frequency;
	_period = 1.0 / frequency;
	_type = type;
}

AmplitudeOscillator::~AmplitudeOscillator()
{

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
	}
}