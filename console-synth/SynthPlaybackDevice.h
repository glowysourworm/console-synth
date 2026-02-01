#pragma once

#include "PlaybackDevice.h"
#include "RtAudio.h"
#include "Synth.h"

class SynthPlaybackDevice : public PlaybackDevice
{
public:

	bool PlaybackDevice::Initialize() override;

	int PlaybackDevice::RtAudioCallback(void* outputBuffer,
		void* inputBuffer,
		unsigned int nFrames,
		double streamTime,
		RtAudioStreamStatus status,
		void* userData) override;

private:

	Synth* _synth;
	bool _initialized;
};

