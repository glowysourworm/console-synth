#include "AudioPlayer.h"
#include "PlaybackDevice.h"
#include "RtAudio.h"
#include "SynthPlaybackDevice.h"
#include "SynthPlayer.h"

SynthPlayer::SynthPlayer()
{
	_device = new SynthPlaybackDevice();
}

SynthPlayer::~SynthPlayer()
{
	AudioPlayer::~AudioPlayer();

	delete _device;
}

bool SynthPlayer::Initialize(RtAudioCallback audioCallback, RtAudioErrorCallback errorCallback)
{
	return AudioPlayer::Initialize(audioCallback, errorCallback) && _device->Initialize();
}

bool SynthPlayer::Tick()
{

	// These allocate memory, so be careful with calls to the synth
	//_synth->Set(60, GetAsyncKeyState(WindowsKeyCodes::J), timeCursor);

	// Also grooms collection of piano notes (Unsets non-engaged notes) (deletes notes)
	//_synth->GetSample(timeCursor);


	//Sleep(LOOP_INCREMENT);

	return true;
}

bool SynthPlayer::Close()
{
	return AudioPlayer::Close();
}

PlaybackDevice* SynthPlayer::GetDevice()
{
	return _device;
}
