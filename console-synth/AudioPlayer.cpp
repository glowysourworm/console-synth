#include "AudioPlayer.h"
#include "Constant.h"
#include "PlaybackDevice.h"
#include "RtAudio.h"
#include <exception>
#include <string>

AudioPlayer::AudioPlayer()
{
	_rtAudio = NULL;
}

AudioPlayer::~AudioPlayer()
{
	if (_rtAudio == NULL)
		return;

	if (_rtAudio->isStreamRunning())
		_rtAudio->abortStream();

	if (_rtAudio->isStreamOpen())
		_rtAudio->closeStream();

	delete _rtAudio;
}

bool AudioPlayer::Initialize(RtAudioCallback audioCallback, RtAudioErrorCallback errorCallback)
{
	_rtAudio = new RtAudio(RtAudio::Api::WINDOWS_WASAPI);

	RtAudio::StreamParameters parameters;
	unsigned int bufferFrames = AUDIO_BUFFER_SIZE;

	RtAudio::StreamOptions options;
	//options.flags |= RTAUDIO_SCHEDULE_REALTIME;					
	options.numberOfBuffers = NUMBER_OF_BUFFERS;
	options.flags |= RTAUDIO_HOG_DEVICE;
	options.flags |= RTAUDIO_MINIMIZE_LATENCY;
	// options.flags |= RTAUDIO_NONINTERLEAVED;

	// STEREO / DEFAULT AUDIO DEVICE
	parameters.deviceId = _rtAudio->getDefaultOutputDevice();
	parameters.firstChannel = 0;
	parameters.nChannels = NUMBER_CHANNELS;

	try
	{
		_rtAudio->openStream(&parameters, NULL, RTAUDIO_FLOAT32, SAMPLING_RATE, &bufferFrames, audioCallback, NULL, &options, errorCallback);
		_rtAudio->startStream();
	}
	catch (std::exception& e)
	{
		return false;
	}

	return true;
}

double AudioPlayer::GetStreamTime() const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return _rtAudio->getStreamTime();
}

bool AudioPlayer::IsStreamRunning() const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return _rtAudio->isStreamRunning();
}
bool AudioPlayer::IsStreamOpen() const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return _rtAudio->isStreamOpen();
}

void AudioPlayer::StartStream() const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	if (_rtAudio->isStreamRunning())
		throw std::exception("RT Audio already running! Must first stop the stream.");

	_rtAudio->startStream();
}

void AudioPlayer::StopStream() const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	_rtAudio->stopStream();
}

void AudioPlayer::CloseStream() const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	_rtAudio->closeStream();
}

RtAudio::DeviceInfo AudioPlayer::GetDeviceInfo(int deviceIndex) const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return _rtAudio->getDeviceInfo(deviceIndex);
}

int AudioPlayer::GetDeviceCount() const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return _rtAudio->getDeviceCount();
}

std::string AudioPlayer::GetVersion() const
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return _rtAudio->getVersion();
}

bool AudioPlayer::Tick()
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return false;
}

PlaybackDevice* AudioPlayer::GetDevice()
{
	return nullptr;
}

bool AudioPlayer::Close()
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return false;
}

RtAudio* AudioPlayer::GetRT()
{
	if (_rtAudio == NULL)
		throw std::exception("RT Audio not initialized:  AudioPlayer.cpp");

	return _rtAudio;
}
