#ifndef COMBFILTER_H
#define COMBFILTER_H

#include "FilterBase.h"
#include <queue>

class CombFilter : FilterBase
{
public:

	// Initializes a feed-back comb filter
	CombFilter(float delaySeconds, float gain, int samplingRate, bool feedbackForm = true);
	~CombFilter();

	float Apply(float sample, float absoluteTime) override;

private:

	std::queue<float>* _buffer;

	bool _feedbackForm;
};

#endif