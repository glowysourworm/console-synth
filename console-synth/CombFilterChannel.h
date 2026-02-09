#ifndef COMBFILTER_H
#define COMBFILTER_H

#include "FilterChannelBase.h"
#include <queue>

class CombFilterChannel : FilterChannelBase
{
public:

	// Initializes a feed-back comb filter
	CombFilterChannel(float delaySeconds, float gain, bool feedback, unsigned int samplingRate);
	~CombFilterChannel();

	float Apply(float inputSample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;

private:

	std::queue<float>* _buffer;

	bool _feedback;
};

#endif