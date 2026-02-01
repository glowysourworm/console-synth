#include "Constant.h"
#include "RtAudio.h"
#include "SynthPlaybackDevice.h"

bool SynthPlaybackDevice::Initialize()
{
	_initialized = true;

	return _initialized;
}

int SynthPlaybackDevice::RtAudioCallback(void* outputBuffer,
	void* inputBuffer,
	unsigned int nFrames,
	double streamTime,
	RtAudioStreamStatus status,
	void* userData)
{
	if (!_initialized)
		return 0;

	if (status == RTAUDIO_INPUT_OVERFLOW)
	{

	}
	else if (status == RTAUDIO_OUTPUT_UNDERFLOW)
	{

	}

	// Output frames should be interleved
	float* buffer = (float*)outputBuffer;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int i = 0; i < nFrames * AUDIO_BUFFER_SIZE; i++)
	{
		// Interleved frames
		for (unsigned int j = 0; j < NUMBER_CHANNELS; j++)
		{
			// Initialize sample to zero
			buffer[(2 * i) + j] = _synth->GetSample(streamTime);
		}
	}

	return 0;
}