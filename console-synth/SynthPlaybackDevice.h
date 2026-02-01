#pragma once

#include "PlaybackDevice.h"
#include "RtAudio.h"
#include "Synth.h"

class SynthPlaybackDevice : public PlaybackDevice
{
public:

	bool Initialize() override;

	int RtAudioCallback(void* outputBuffer,
		void* inputBuffer,
		unsigned int nFrames,
		double streamTime,
		RtAudioStreamStatus status,
		void* userData) override;

	void SetNote(int midiNumber, bool pressed, double streamTime);
	bool GetNote(int midiNumber) const;

private:

	Synth* _synth;
	bool _initialized;
};

