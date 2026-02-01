#ifndef REVERB_H
#define REVERB_H

#include "AllPassFilter.h"
#include "ButterworthFilter.h"
#include "CombFilter.h"
#include "Constant.h"

// Implements Schroeder Reverb
//
class Reverb
{
public:
	Reverb(float delaySeconds, float gain, int samplingRate);
	~Reverb();

	float Apply(float sample);

private:
	CombFilter** _combFilters;
	AllPassFilter** _allPassFilters;
	ButterworthFilter* _lowPassFilter;
};

#endif

