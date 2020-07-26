#ifndef REVERB_H
#define REVERB_H

#include "CombFilter.h"
#include "AllPassFilter.h"

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

	int _combCount;
	int _allPassCount;
};

#endif

