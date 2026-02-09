#include "AllPassFilterChannel.h"
#include "ButterworthFilterChannel.h"
#include "CombFilterChannel.h"
#include "Constant.h"
#include "FilterChannelBase.h"
#include "ReverbChannel.h"
#include <cmath>

ReverbChannel::ReverbChannel(float delaySeconds, float gain, int samplingRate) : FilterChannelBase(gain, samplingRate)
{
	_combFilters = new CombFilterChannel * [REVERB_COMB_SIZE];
	_allPassFilters = new AllPassFilterChannel * [REVERB_ALLPASS_SIZE];
	_lowPassFilter = new ButterworthFilterChannel(samplingRate, 1);

	// https://ccrma.stanford.edu/~jos/pasp/Schroeder_Reverberators.html
	//
	float allPassDelayBase = delaySeconds;
	// float mutuallyPrimeNumbers[REVERB_COMB_SIZE] = { 23 / 23.0, 17 / 23.0, 13 / 23.0, 7 / 23.0 };
	float schroederCoefficients[REVERB_COMB_SIZE] = { 0.046, 0.057, 0.041, 0.054 };

	// FIGURE OUT EFFECTS OF STAGGERING THE COMBS
	for (int i = 0; i < REVERB_COMB_SIZE; i++)
		_combFilters[i] = new CombFilterChannel(delaySeconds * schroederCoefficients[i], gain, samplingRate, false);

	for (int i = 0; i < REVERB_ALLPASS_SIZE; i++)
		_allPassFilters[i] = new AllPassFilterChannel(allPassDelayBase / powf(3, i), gain, samplingRate);

	_lowPassFilter->Set(8000, 0.1);

}
ReverbChannel::~ReverbChannel()
{
	for (int i = 0; i < REVERB_COMB_SIZE; i++)
		delete _combFilters[i];

	for (int i = 0; i < REVERB_ALLPASS_SIZE; i++)
		delete _allPassFilters[i];

	delete[] _combFilters;
	delete[] _allPassFilters;
}
float ReverbChannel::Apply(float sample, float absoluteTime)
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

bool ReverbChannel::HasOutput(float absoluteTime) const
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
