#ifndef BUTTERWORTHFILTER_H
#define BUTTERWORTHFILTER_H

#define BUDDA_Q_SCALE 6.f

#include "FilterBase.h"

class ButterworthFilter : public FilterBase
{
public:

	ButterworthFilter(int samplingRate, float gain);
	~ButterworthFilter();

	float Apply(float sample, float absoluteTime) override;

	void Set(float cutoffFrequency, float resonance);

private:

	void Initialize();

private:

	float outputGain;								// Second gain applied depending on cutoff / resonance
	float t0, t1, t2, t3;
	float coef0, coef1, coef2, coef3;
	float history1, history2, history3, history4;
	float min_cutoff, max_cutoff;
};

#endif
