#include "Constant.h"
#include "RtAudio.h"
#include "Synth.h"
#include "SynthPlaybackDevice.h"

bool SynthPlaybackDevice::Initialize()
{
	_initialized = true;

	_synth = new Synth();

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
		int foo = 34;
	}
	else if (status == RTAUDIO_OUTPUT_UNDERFLOW)
	{
		int foowewe = 2;
	}

	// Output frames should be interleved
	float* buffer = (float*)outputBuffer;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int i = 0; i < nFrames; i++)
	{
		double absoluteTime = streamTime + (i / (double)SAMPLING_RATE);

		// Interleved frames
		for (unsigned int j = 0; j < NUMBER_CHANNELS; j++)
		{
			// Initialize sample to zero
			double sample = _synth->GetSample(absoluteTime);

			buffer[(2 * i) + j] = sample;
		}
	}

	return 0;
}

void SynthPlaybackDevice::SetNote(int midiNumber, bool pressed, double streamTime)
{
	_synth->Set(midiNumber, pressed, streamTime);
}

bool SynthPlaybackDevice::GetNote(int midiNumber) const
{
	return _synth->IsSet(midiNumber);
}
