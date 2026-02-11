#pragma once

#include "MixerChannel.h"
#include "PlaybackFrame.h"
#include <map>
class Mixer
{
public:

	// Creates a mixer with the specified number of channels
	Mixer(float gain, float leftRight, unsigned int numberOfChannels);
	~Mixer();

	void Set(float gain, float leftRight);

	bool HasChannel(int channelKey) const;
	void SetChannel(int channelKey, float gain, float leftRight);
	void SetSample(int channelKey, PlaybackFrame* source);
	void ClearChannel(int channelKey);

	PlaybackFrame* GetFrame(int channelKey);

	void MixOutput(PlaybackFrame* destination);

private:

	std::map<int, MixerChannel*>* _channels;

	float _gain;
	float _leftRight;
	unsigned int _numberOfChannels;
};