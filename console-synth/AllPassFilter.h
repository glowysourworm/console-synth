#ifndef ALLPASSFILTER_H
#define ALLPASSFILTER_H

#include "FilterBase.h"

#include <queue>

class AllPassFilter : public FilterBase
{
public:
	AllPassFilter(float delaySeconds, float gain, int samplingRate);
	~AllPassFilter();

	float Apply(float sample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;

private:

	int _bufferSize;
	std::queue<float>* _delayedInput;
	std::queue<float>* _delayedOutput;
};

#endif

