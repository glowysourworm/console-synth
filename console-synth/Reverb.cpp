#include "AllPassFilter.h"
#include "ButterworthFilter.h"
#include "CombFilter.h"
#include "Constant.h"
#include "FilterBase.h"
#include "Reverb.h"
#include <cmath>

Reverb::Reverb(float delaySeconds, float gain, int samplingRate) : FilterBase(gain, samplingRate)
{
	_combFilters = new CombFilter * [REVERB_COMB_SIZE];
	_allPassFilters = new AllPassFilter * [REVERB_ALLPASS_SIZE];
	_lowPassFilter = new ButterworthFilter(SAMPLING_RATE, 1);

	// https://ccrma.stanford.edu/~jos/pasp/Schroeder_Reverberators.html
	//
	float allPassDelayBase = delaySeconds;
	// float mutuallyPrimeNumbers[REVERB_COMB_SIZE] = { 23 / 23.0, 17 / 23.0, 13 / 23.0, 7 / 23.0 };
	float schroederCoefficients[REVERB_COMB_SIZE] = { 0.046, 0.057, 0.041, 0.054 };

	// FIGURE OUT EFFECTS OF STAGGERING THE COMBS
	for (int i = 0; i < REVERB_COMB_SIZE; i++)
		_combFilters[i] = new CombFilter(delaySeconds * schroederCoefficients[i], gain, samplingRate, false);

	for (int i = 0; i < REVERB_ALLPASS_SIZE; i++)
		_allPassFilters[i] = new AllPassFilter(allPassDelayBase / powf(3, i), gain, samplingRate);

	_lowPassFilter->Set(8000, 0.1);

}
Reverb::~Reverb()
{
	for (int i = 0; i < REVERB_COMB_SIZE; i++)
		delete _combFilters[i];

	for (int i = 0; i < REVERB_ALLPASS_SIZE; i++)
		delete _allPassFilters[i];

	delete[] _combFilters;
	delete[] _allPassFilters;
}
float Reverb::Apply(float sample, float absoluteTime)
{
	// https://ccrma.stanford.edu/~jos/pasp/Schroeder_Reverberators.html
	//
	// Other techniques to try
	//
	// https://www.musicdsp.org/en/latest/Effects/45-reverberation-techniques.html
	//

	float output = sample;

	// All pass taps
	//
	//float outputA = _allPassFilters[0]->Apply(output, absoluteTime);
	//float outputB = _allPassFilters[1]->Apply(outputA, absoluteTime);
	//float outputC = _allPassFilters[2]->Apply(outputB, absoluteTime);
	//float outputD = _allPassFilters[3]->Apply(outputC, absoluteTime);

	// Series all pass filters
	for (int i = 0; i < REVERB_ALLPASS_SIZE; i++)
		output = _allPassFilters[i]->Apply(output, absoluteTime);

	// Calculate comb filter outputs
	float outputA = _combFilters[0]->Apply(output, absoluteTime);
	float outputB = _combFilters[1]->Apply(output, absoluteTime);
	float outputC = _combFilters[2]->Apply(output, absoluteTime);
	float outputD = _combFilters[3]->Apply(output, absoluteTime);	
	
	// Apply mixing matrix to comb outputs
	float mixedOutput = (0.5 * output + 0.6 * outputA + 0.7 * outputB + 0.85 * outputC + outputD);

	return _lowPassFilter->Apply(mixedOutput, absoluteTime);
}

bool Reverb::HasOutput(float absoluteTime) const
{
	// Ringing based on all pass filters
	for (int index = 0; index < REVERB_ALLPASS_SIZE; index++)
	{
		if (_allPassFilters[index]->HasOutput(absoluteTime))
			return true;
	}

	// Ringing based on comb filters
	for (int index = 0; index < REVERB_COMB_SIZE; index++)
	{
		if (_combFilters[index]->HasOutput(absoluteTime))
			return true;
	}

	return false;
}
