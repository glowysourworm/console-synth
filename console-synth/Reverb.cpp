#include "Reverb.h"

Reverb::Reverb(float delaySeconds, float gain, int samplingRate)
{
	_combCount = 4;
	_allPassCount = 0;

	_combFilters = new CombFilter * [_combCount];
	_allPassFilters = new AllPassFilter * [_allPassCount];

	// FIGURE OUT EFFECTS OF STAGGERING THE COMBS
	for (int i = 0; i < _combCount; i++)
		_combFilters[i] = new CombFilter(((i + 1.0) * delaySeconds) / 4.0, gain, samplingRate);

	for (int i = 0; i < _allPassCount; i++)
		_allPassFilters[i] = new AllPassFilter(((i + 1.0) * delaySeconds) / 4.0, gain, samplingRate);

}
Reverb::~Reverb()
{
	for (int i = 0; i < _combCount; i++)
		delete _combFilters[i];

	for (int i = 0; i < _allPassCount; i++)
		delete _allPassFilters[i];

	delete[] _combFilters;
	delete[] _allPassFilters;
}
float Reverb::Apply(float sample)
{
	// https://medium.com/the-seekers-project/coding-a-basic-reverb-algorithm-part-2-an-introduction-to-audio-programming-4db79dd4e325
	//
	// W comb filters followed by Z all-pass filters
	//

	float output = sample;

	for (int i = 0; i < _combCount; i++)
		output = _combFilters[i]->Apply(output);

	for (int i = 0; i < _allPassCount; i++)
		output = _allPassFilters[i]->Apply(output);

	return output;
}