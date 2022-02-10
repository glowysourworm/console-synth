#ifndef ALLPASSFILTER_H
#define ALLPASSFILTER_H

#include <queue>

class AllPassFilter
{
public:
	AllPassFilter(float delaySeconds, float gain, int samplingRate);
	~AllPassFilter();

	float Apply(float sample);

private:

	float* _delayedInput;
	float* _delayedOutput;

	float _gain;
};

#endif

