#include "Mixer.h"
#include "MixerChannel.h"
#include "PlaybackFrame.h"
#include <exception>
#include <map>

Mixer::Mixer(float gain, float leftRight, unsigned int numberOfChannels)
{
	_channels = new std::map<int, MixerChannel*>();

	_gain = gain;
	_leftRight = leftRight;
	_numberOfChannels = numberOfChannels;
}
Mixer::~Mixer()
{
	for (auto iter = _channels->begin(); iter != _channels->end(); ++iter)
	{
		delete iter->second;
	}

	delete _channels;
}

void Mixer::Set(float gain, float leftRight)
{
	_gain = gain;
	_leftRight = leftRight;
}

bool Mixer::HasChannel(int channelKey) const
{
	return _channels->contains(channelKey);
}

void Mixer::SetChannel(int channelKey, float gain, float leftRight)
{
	// Channels not using L/R balance. The output will apply its L/R parameter to the sample frame.
	if (!_channels->contains(channelKey))
		_channels->insert({ channelKey, new MixerChannel(_numberOfChannels, gain, leftRight) });

	else
		_channels->at(channelKey)->Set(gain, leftRight);
}

void Mixer::SetSample(int channelKey, PlaybackFrame* source)
{
	if (!_channels->contains(channelKey))
		throw new std::exception("Mixer channel not contained within the mixer!");

	_channels->at(channelKey)->SetSample(source);
}

void Mixer::ClearChannel(int channelKey)
{
	if (!_channels->contains(channelKey))
		throw new std::exception("Mixer channel not contained within the mixer!");

	_channels->erase(channelKey);
}

void Mixer::MixOutput(PlaybackFrame* dest)
{
	// Mix channels into the current frame
	for (auto iter = _channels->begin(); iter != _channels->end(); ++iter)
	{
		if (iter->second->GetFrame()->HasOutput())
			iter->second->MixSample(dest);
	}

	// Apply Output Parameters
	//for (int index = 0; index < dest->GetChannelCount(); index++)
	//{
	//	// Left
	//	if (index < dest->GetChannelCount() / 2)
	//		dest->SetSample(index, dest->GetSample(index) * _gain * _leftRight);

	//	// Right
	//	else
	//		dest->SetSample(index, dest->GetSample(index) * _gain * (1 - _leftRight));
	//}
}

PlaybackFrame* Mixer::GetFrame(int channelKey)
{
	return _channels->at(channelKey)->GetFrame();
}
