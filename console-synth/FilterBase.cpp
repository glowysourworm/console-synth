#include "FilterBase.h"

FilterBase::FilterBase(float gain, int samplingRate)
{
	_gain = gain;
	_samplingRate = samplingRate;
}

FilterBase::~FilterBase()
{
}
float FilterBase::GetSamplingRate() const
{
	return _samplingRate;
}
float FilterBase::GetGain() const
{
	return _gain;
}