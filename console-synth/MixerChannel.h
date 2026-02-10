#pragma once

#include "PlaybackFrame.h"

class MixerChannel
{
public:

	MixerChannel(unsigned int numberOfChannels, float gain, float leftRight)
	{
		this->frame = new PlaybackFrame(numberOfChannels);
		this->gain = gain;
		this->leftRight = leftRight;
	}

	~MixerChannel()
	{
		delete this->frame;
	}

	void Set(float gain, float leftRight)
	{
		this->gain = gain;
		this->leftRight = leftRight;
	}

	PlaybackFrame* GetFrame()
	{
		return this->frame;
	}

	void SetSample(const PlaybackFrame* source)
	{
		this->frame->SetFrame(source);
	}

	void MixSample(PlaybackFrame* dest)
	{
		for (int index = 0; index < frame->GetChannelCount(); index++)
		{
			float source;
			float destination = dest->GetSample(index);

			// Left
			if (index < frame->GetChannelCount() / 2)
				source = frame->GetSample(index) * leftRight * gain;

			// Right
			else
				source = frame->GetSample(index) * (1 - leftRight) * gain;

			dest->SetSample(index, source + destination /* - (source * destination) */);
		}
	}

private:

	PlaybackFrame* frame;
	float gain;
	float leftRight;
};