#pragma once

#include "Constant.h"
#include "PlaybackBuffer.h"
#include "PlaybackDevice.h"
#include "Synth.h"
#include "SynthConfiguration.h"

template<SignalValue TSignal>
class SynthPlaybackDevice : public PlaybackDevice<TSignal>
{
public:

	bool Initialize() override;

	int WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackBuffer, unsigned int numberOfFrames, double streamTime) override;

	void SetNote(int midiNumber, bool pressed, double streamTime);
	bool GetNote(int midiNumber) const;
	bool HasNote(int midiNumber) const;
	void ClearUnused(double streamTime);
	void UpdateSynth(const SynthConfiguration& configuration);

public:

	/// <summary>
	/// Gets the currently active notes in the synth as an array of midi note numbers
	/// </summary>
	void GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const;

private:

	Synth* _synth;
	bool _initialized;
};


template<SignalValue TSignal>
bool SynthPlaybackDevice<TSignal>::Initialize()
{
	_initialized = true;

	// Attack / Decay / Sustain / Release / Attack Peak / Sustain Peak
	//
	SynthConfiguration configuration;

	_synth = new Synth(configuration);

	return _initialized;
}

template<SignalValue TSignal>
int SynthPlaybackDevice<TSignal>::WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackBuffer, unsigned int numberOfFrames, double streamTime)
{
	if (!_initialized)
		return 0;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int frameIndex = 0; frameIndex < numberOfFrames; frameIndex++)
	{
		double absoluteTime = streamTime + (frameIndex / (double)playbackBuffer->GetSamplingRate());

		// Interleved frames
		for (unsigned int channelIndex = 0; channelIndex < playbackBuffer->GetNumberOfChannels(); channelIndex++)
		{
			// Initialize sample to zero
			TSignal sample = (TSignal)_synth->GetSample(absoluteTime);

			// Set output sample
			playbackBuffer->SetBufferFrame(sample, frameIndex, channelIndex);
		}
	}

	return numberOfFrames;
}

template<SignalValue TSignal>
void SynthPlaybackDevice<TSignal>::SetNote(int midiNumber, bool pressed, double streamTime)
{
	_synth->Set(midiNumber, pressed, streamTime);
}

template<SignalValue TSignal>
bool SynthPlaybackDevice<TSignal>::GetNote(int midiNumber) const
{
	return _synth->IsSet(midiNumber);
}

template<SignalValue TSignal>
bool SynthPlaybackDevice<TSignal>::HasNote(int midiNumber) const
{
	return _synth->HasNote(midiNumber);
}

template<SignalValue TSignal>
void SynthPlaybackDevice<TSignal>::ClearUnused(double streamTime)
{
	_synth->Clear(streamTime);
}

template<SignalValue TSignal>
void SynthPlaybackDevice<TSignal>::UpdateSynth(const SynthConfiguration& configuration)
{
	_synth->SetConfiguration(configuration);
}

template<SignalValue TSignal>
void SynthPlaybackDevice<TSignal>::GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const
{
	_synth->GetNotes(array, arrayLength);
}