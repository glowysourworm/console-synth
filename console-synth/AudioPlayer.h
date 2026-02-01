#pragma once

#include "PlaybackDevice.h"
#include "RtAudio.h"
#include <string>

class AudioPlayer
{
public:

	AudioPlayer();
	~AudioPlayer();

	// RT Audio
	bool IsStreamRunning() const;
	bool IsStreamOpen() const;

	void StopStream() const;
	void CloseStream() const;

	RtAudio::DeviceInfo GetDeviceInfo(int deviceIndex) const;
	int GetDeviceCount() const;
	std::string GetVersion() const;

public:
	virtual bool Initialize(RtAudioCallback audioCallback, RtAudioErrorCallback errorCallback);
	virtual bool Tick() = 0;
	virtual PlaybackDevice* GetDevice();
	virtual bool Close();

protected:

	RtAudio* GetRT();

private:

	RtAudio* _rtAudio;
};

