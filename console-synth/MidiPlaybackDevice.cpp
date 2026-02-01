#include "Constant.h"
#include "MidiPlaybackDevice.h"
#include "RtAudio.h"

bool MidiPlaybackDevice::Initialize()
{
	_initialized = true;

	return _initialized;
}

int MidiPlaybackDevice::RtAudioCallback(void* outputBuffer,
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
	float sampleSize = (1.0 / (float)SAMPLING_RATE);
	float systemTime = (float)_frameIndex / (float)SAMPLING_RATE;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int i = 0; (i < nFrames) && (_frameIndex < _waveBufferLength); i++)
	{
		float absoluteTime = (_frameIndex++) * sampleSize;
		// float sample = _synth->GetSample(absoluteTime);
		float sample = _waveBuffer[_frameIndex + i];

		// Interleved frames
		for (unsigned int j = 0; j < NUMBER_CHANNELS; j++)
		{
			// Initialize sample to zero
			buffer[(2 * i) + j] = sample;
		}
	}

	return 0;
}