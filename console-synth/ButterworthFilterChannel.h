#pragma once
#define BUDDA_Q_SCALE 6.f

#include "FilterChannelBase.h"

class ButterworthFilterChannel : public FilterChannelBase
{
public:

	ButterworthFilterChannel(int samplingRate, float gain);
	~ButterworthFilterChannel();

	float Apply(float inputSample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;

	void Set(float cutoffFrequency, float resonance);

private:

	float outputGain;								// Second gain applied depending on cutoff / resonance
	float t0, t1, t2, t3;
	float coef0, coef1, coef2, coef3;
	float history1, history2, history3, history4;
	float min_cutoff, max_cutoff;
};
