#pragma once

#include "PlaybackDevice.h"
#include "Synth.h"

class MidiPlaybackDevice : public PlaybackDevice
{
public:

	bool Initialize() override;

	int StreamCallback(void* outputBuffer,
		unsigned int numberOfFrames,
		unsigned int numberOfChannels,
		double streamTime) override;

private:

	Synth* _synth;
	int _frameIndex;
	bool _initialized;
	float* _waveBuffer;
	int _waveBufferLength;

};

