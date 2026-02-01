#pragma once

#include "AudioPlayer.h"
#include "MidiPlaybackDevice.h"
#include "RtAudio.h"

class MidiFilePlayer : public AudioPlayer
{
public:

	MidiFilePlayer();
	~MidiFilePlayer();

public:
	bool Initialize(RtAudioCallback audioCallback, RtAudioErrorCallback errorCallback) override;
	bool Tick() override;
	bool Close() override;

private:

	MidiPlaybackDevice* _device;
};

