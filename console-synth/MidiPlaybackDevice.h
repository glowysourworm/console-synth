#pragma once

#include "PlaybackDevice.h"
#include "RtAudio.h"
#include "Synth.h"

class MidiPlaybackDevice : public PlaybackDevice
{
public:

	bool Initialize() override;

	int RtAudioCallback(void* outputBuffer,
		void* inputBuffer,
		unsigned int nFrames,
		double streamTime,
		RtAudioStreamStatus status,
		void* userData) override;

private:

	Synth* _synth;
	int _frameIndex;
	bool _initialized;
	float* _waveBuffer;
	int _waveBufferLength;

};

