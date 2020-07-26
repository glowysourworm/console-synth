#ifndef COMBFILTER_H
#define COMBFILTER_H

#include <queue>

class CombFilter
{
public:
	CombFilter(float delaySeconds, float gain, int samplingRate);
	~CombFilter();

	float Apply(float sample);

private:

	std::queue<float>* _delayedOutput;

	float _gain;
};

#endif