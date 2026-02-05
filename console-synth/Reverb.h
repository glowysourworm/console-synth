#ifndef REVERB_H
#define REVERB_H

#include "AllPassFilter.h"
#include "ButterworthFilter.h"
#include "CombFilter.h"
#include "FilterBase.h"

// Implements Schroeder Reverb
//
class Reverb : public FilterBase
{
public:
	Reverb(float delaySeconds, float gain, int samplingRate);
	~Reverb();

	float Apply(float sample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;

private:
	CombFilter** _combFilters;
	AllPassFilter** _allPassFilters;
	ButterworthFilter* _lowPassFilter;
};

#endif

