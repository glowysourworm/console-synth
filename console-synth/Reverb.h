#ifndef REVERB_H
#define REVERB_H

#include "Constant.h"
#include "CombFilter.h"
#include "AllPassFilter.h"
#include "ButterworthFilter.h"

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

