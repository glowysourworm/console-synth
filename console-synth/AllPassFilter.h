#ifndef ALLPASSFILTER_H
#define ALLPASSFILTER_H

#include "FilterBase.h"

class AllPassFilter : public FilterBase
{
public:
	AllPassFilter(float delaySeconds, float gain, int samplingRate);
	~AllPassFilter();

	float Apply(float sample, float absoluteTime) override;

private:

	float* _delayedInput;
	float* _delayedOutput;

	float _gain;
};

#endif

