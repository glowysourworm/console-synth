#include "OscillatorBase.h"
#include "SignalBase.h"

OscillatorBase::OscillatorBase(float frequency) : SignalBase()
{
	_frequency = frequency;
	_period = 1 / frequency;
}
OscillatorBase::~OscillatorBase()
{
}
void OscillatorBase::Set(float frequency)
{
	_frequency = frequency;
	_period = 1 / frequency;
}
float OscillatorBase::GetFrequency() const
{
	return _frequency;
}
float OscillatorBase::GetPeriod() const
{
	return _period;
}
