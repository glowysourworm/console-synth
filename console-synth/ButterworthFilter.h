#ifndef BUTTERWORTHFILTER_H
#define BUTTERWORTHFILTER_H

#define BUDDA_Q_SCALE 6.f

#include <math.h>

class ButterworthFilter
{
public:

	ButterworthFilter(int samplingRate, float gain);
	~ButterworthFilter();

	float Apply(float sample);

	void Set(float cutoffFrequency, float resonance);

private:
	void SetSampleRate(float fs);

private:

	float t0, t1, t2, t3;
	float coef0, coef1, coef2, coef3;
	float history1, history2, history3, history4;
	float gain;
	float min_cutoff, max_cutoff;
};

#endif
