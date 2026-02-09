#include "FilterBase.h"

FilterBase::FilterBase(float gain, unsigned int numberOfChannels, unsigned int samplingRate)
{
	_gain = gain;
	_numberOfChannels = numberOfChannels;
	_samplingRate = samplingRate;
}

FilterBase::~FilterBase()
{
}
unsigned int FilterBase::GetNumberOfChannels() const
{
	return _numberOfChannels;
}
unsigned int FilterBase::GetSamplingRate() const
{
	return _samplingRate;
}
float FilterBase::GetGain() const
{
	return _gain;
}