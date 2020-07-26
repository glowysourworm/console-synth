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

	std::queue<float>* _delayedInput;
	std::queue<float>* _delayedOutput;

	float _gain;
};

#endif

