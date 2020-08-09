#include "Mixer.h"

Mixer::Mixer(int channels)
{
	_channels = new std::vector<Mixer::MixerChannel>(channels);

	for (int i = 0; i < channels; i++)
		_channels->push_back(Mixer::MixerChannel(0, 0));
}
Mixer::~Mixer()
{
	delete _channels;
}

void Mixer::SetChannel(int index, float sample, float relativeMix)
{
	_channels->at(index).Set(sample, relativeMix);
}

float Mixer::Get()
{
	float result = 0;
	float totalMixLevel = 0;

	// Sum up the total mix level
	for (int i = 0; i < (int)_channels->size(); i++)
	{
		totalMixLevel += _channels->at(i).mixLevel;
	}

	// Do a relative weighting
	for (int i = 0; i < (int)_channels->size(); i++)
	{
		result += (_channels->at(i).sample * _channels->at(i).mixLevel) / totalMixLevel;
	}

	return result;
}