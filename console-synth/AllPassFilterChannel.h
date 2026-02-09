#ifndef ALLPASSFILTER_H
#define ALLPASSFILTER_H

#include "FilterChannelBase.h"
#include "PlaybackFrame.h"

#include <queue>

class AllPassFilterChannel : public FilterChannelBase
{
public:
	AllPassFilterChannel(float delaySeconds, float gain, unsigned int samplingRate);
	~AllPassFilterChannel();

	float Apply(float inputSample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;

private:

	int _bufferSize;
	std::queue<float>* _delayedInput;
	std::queue<float>* _delayedOutput;
};

#endif

