#pragma once
#include "FilterChannelBase.h"
#include "SynthConfiguration.h"
#include <queue>

class CombFilterChannel : FilterChannelBase
{
public:

	// Initializes a feed-back comb filter
	CombFilterChannel(float delaySeconds, float gain, bool feedback, unsigned int samplingRate);
	~CombFilterChannel();

	void Set(bool feedback);

	float Apply(float inputSample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;
	void SetConfiguration(const SynthConfiguration* configuration) override;

private:

	std::queue<float>* _buffer;

	bool _feedback;
};