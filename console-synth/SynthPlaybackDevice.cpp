#include "Constant.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"

bool SynthPlaybackDevice::Initialize()
{
	_initialized = true;

	// Attack / Decay / Sustain / Release / Attack Peak / Sustain Peak
	//
	SynthConfiguration configuration;

	_synth = new Synth(configuration);

	return _initialized;
}

int SynthPlaybackDevice::StreamCallback(void* outputBuffer, unsigned int numberOfFrames, unsigned int numberOfChannels, double streamTime)
{
	if (!_initialized)
		return 0;

	// Output frames should be interleved
	float* buffer = (float*)outputBuffer;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int i = 0; i < numberOfFrames; i++)
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

bool SynthPlaybackDevice::HasNote(int midiNumber) const
{
	return _synth->HasNote(midiNumber);
}

void SynthPlaybackDevice::ClearUnused(double streamTime)
{
	_synth->Clear(streamTime);
}

void SynthPlaybackDevice::UpdateSynth(const SynthConfiguration& configuration)
{
	_synth->SetConfiguration(configuration);
}

void SynthPlaybackDevice::GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const
{
	_synth->GetNotes(array, arrayLength);
}
