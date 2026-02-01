#pragma once

#include "AudioPlayer.h"
#include "PlaybackDevice.h"
#include "RtAudio.h"
#include "SynthPlaybackDevice.h"

class SynthPlayer : public AudioPlayer
{
public:
	SynthPlayer();
	~SynthPlayer();

public:

	bool Initialize(RtAudioCallback audioCallback, RtAudioErrorCallback errorCallback) override;
	bool Tick() override;
	bool Close() override;
	PlaybackDevice* GetDevice() override;

private:

	SynthPlaybackDevice* _device;
};

