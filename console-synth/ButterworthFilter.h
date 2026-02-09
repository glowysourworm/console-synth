#pragma once
#define BUDDA_Q_SCALE 6.f

#include "ButterworthFilterChannel.h"
#include "FilterBase.h"
#include "PlaybackFrame.h"

class ButterworthFilter : public FilterBase
{
public:

	ButterworthFilter(float gain, unsigned int numberOfChannels, unsigned int samplingRate);
	~ButterworthFilter();

	void GetSample(PlaybackFrame* frame, float absoluteTime) override;
	virtual bool HasOutput(float absoluteTime) const = 0;

	void Set(float cutoffFrequency, float resonance);

private:

	ButterworthFilterChannel** _filterChannels;
};