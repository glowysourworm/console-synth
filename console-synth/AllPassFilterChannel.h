#pragma once
#include "FilterChannelBase.h"

#include "SynthConfiguration.h"
#include <queue>

class AllPassFilterChannel : public FilterChannelBase
{
public:
	AllPassFilterChannel(float delaySeconds, float gain, unsigned int samplingRate);
	~AllPassFilterChannel();

	void Set(float delaySeconds, float gain, unsigned int samplingRate);

	float Apply(float inputSample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;
	void SetConfiguration(const SynthConfiguration* configuration) override;

private:

	int _bufferSize;
	std::queue<float>* _delayedInput;
	std::queue<float>* _delayedOutput;
};