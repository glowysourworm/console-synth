#pragma once

#include "Constant.h"
#include "PlaybackBuffer.h"
#include "PlaybackDevice.h"
#include "PlaybackParameters.h"
#include "Synth.h"
#include "SynthConfiguration.h"

template<SignalValue TSignal>
class SynthPlaybackDevice : public PlaybackDevice<TSignal>
{
public:

	SynthPlaybackDevice();
	~SynthPlaybackDevice();

	bool Initialize(const PlaybackParameters& parameters) override;

	int WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime) override;
	int WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackBuffer, unsigned int numberOfFrames, double streamTime) override;

	void SetNote(int midiNumber, bool pressed, double streamTime, unsigned int samplingRate);
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

	PlaybackParameters* _streamParameters;
	Synth* _synth;
	bool _initialized;
};


template<SignalValue TSignal>
SynthPlaybackDevice<TSignal>::SynthPlaybackDevice()
{
	_streamParameters = new PlaybackParameters();
}

template<SignalValue TSignal>
SynthPlaybackDevice<TSignal>::~SynthPlaybackDevice()
{
	delete _streamParameters;
}

template<SignalValue TSignal>
bool SynthPlaybackDevice<TSignal>::Initialize(const PlaybackParameters& parameters)
{
	_initialized = true;
	_streamParameters->numberOfChannels = parameters.numberOfChannels;
	_streamParameters->samplingRate = parameters.samplingRate;
	_streamParameters->outputBufferFrameSize = parameters.outputBufferFrameSize;

	// Attack / Decay / Sustain / Release / Attack Peak / Sustain Peak
	//
	SynthConfiguration configuration;

	_synth = new Synth(configuration, parameters.samplingRate);

	return _initialized;
}

template<SignalValue TSignal>
inline int SynthPlaybackDevice<TSignal>::WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime)
{
	if (!_initialized)
		return 0;

	TSignal* outputBuffer = (TSignal*)playbackBuffer;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int frameIndex = 0; frameIndex < numberOfFrames; frameIndex++)
	{
		double absoluteTime = streamTime + (frameIndex / (double)_streamParameters->samplingRate);

		// Interleved frames
		for (unsigned int channelIndex = 0; channelIndex < _streamParameters->numberOfChannels; channelIndex++)
		{
			// Initialize sample to zero
			TSignal sample = (TSignal)_synth->GetSample(absoluteTime);

			// Set output sample
			outputBuffer[(2 * frameIndex) + channelIndex] = sample;
		}
	}

	return 0;
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

	return 0;
}

template<SignalValue TSignal>
void SynthPlaybackDevice<TSignal>::SetNote(int midiNumber, bool pressed, double streamTime, unsigned int samplingRate)
{
	_synth->Set(midiNumber, pressed, streamTime, samplingRate);
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
	_synth->SetConfiguration(configuration, _streamParameters->samplingRate);
}

template<SignalValue TSignal>
void SynthPlaybackDevice<TSignal>::GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const
{
	_synth->GetNotes(array, arrayLength);
}