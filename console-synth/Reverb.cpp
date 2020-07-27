#include "Reverb.h"

Reverb::Reverb(float delaySeconds, float gain, int samplingRate)
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
float Reverb::Apply(float sample)
{
	// https://ccrma.stanford.edu/~jos/pasp/Schroeder_Reverberators.html
	//

	float output = sample;

	// Series all pass filters
	for (int i = 0; i < REVERB_ALLPASS_SIZE; i++)
		output = _allPassFilters[i]->Apply(output);

	// Calculate comb filter outputs
	float outputA = _combFilters[0]->Apply(output);
	float outputB = _combFilters[1]->Apply(output);
	float outputC = _combFilters[2]->Apply(output);
	float outputD = _combFilters[3]->Apply(output);

	// output = 0.25 * (outputA + outputB + outputC + outputD);



	// Apply mixing matrix to comb outputs
	
	// return output;
	// return _lowPassFilter->Apply(output);

	return 0.25 * (outputA + outputB + outputC + outputD);
}