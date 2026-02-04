#include "Mixer.h"
#include <exception>
#include <map>

Mixer::Mixer()
{
	_channels = new std::map<int, Mixer::MixerChannel>();
}
Mixer::~Mixer()
{
	delete _channels;
}

void Mixer::SetChannel(int channelKey, float sample, float relativeMix)
{
	if (_channels->contains(channelKey))
		_channels->at(channelKey).Set(sample, relativeMix);

	else
		_channels->insert({ channelKey, Mixer::MixerChannel(sample, relativeMix) });
}

void Mixer::ClearChannel(int channelKey)
{
	if (!_channels->contains(channelKey))
		throw new std::exception("Mixer channel not contained within the mixer!");

	_channels->erase(channelKey);
}

float Mixer::Get()
{
	int engagedChannels = 0;
	float result = 0;
	float totalMixLevel = 0;

	// Sum up the total mix level
	//for (int i = 0; i < (int)_channels->size(); i++)
	//{
	//	totalMixLevel += _channels->at(i).mixLevel;
	//}

	// Do a relative weighting
	for (auto iter = _channels->begin(); iter != _channels->end(); ++iter)
	{
		result += iter->second.sample * iter->second.mixLevel;
	}

	if (_channels->size() > 0)
		result = result / (double)_channels->size();
	else
		result = 0;

	return result;
}