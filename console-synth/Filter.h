#pragma once

#include "FilterBase.h"
#include "FilterChannelBase.h"
#include "PlaybackFrame.h"
#include <vector>

class Filter : public FilterBase
{
public:

	Filter(float gain, unsigned int numberOfChannels, unsigned int samplingRate);
	virtual ~Filter() override;

	void AddChannel(FilterChannelBase* filter);

	virtual void GetSample(PlaybackFrame* frame, float absoluteTime) override;
	virtual bool HasOutput(float absoluteTime) const override;

private:

	std::vector<FilterChannelBase*>* _channels;
};

