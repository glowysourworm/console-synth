#pragma once

#include "Accumulator.h"
#include "Constant.h"
#include "PlaybackBuffer.h"
#include "PlaybackDevice.h"
#include "PlaybackFrame.h"
#include "PlaybackParameters.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include <cmath>

template<SignalValue TSignal>
class SynthPlaybackDevice : public PlaybackDevice<TSignal>
{
public:

	SynthPlaybackDevice();
	~SynthPlaybackDevice();

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters& parameters) override;

	int WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime) override;
	//int WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackBuffer, unsigned int numberOfFrames, double streamTime) override;

	void SetNote(int midiNumber, bool pressed, double streamTime);
	bool GetNote(int midiNumber) const;
	bool HasNote(int midiNumber) const;
	void UpdateSynth(const SynthConfiguration* configuration);

	/// <summary>
	/// Returns average output for specified channel from the last frame buffer write
	/// </summary>
	TSignal GetOutput(int channelIndex) const;

public:

	/// <summary>
	/// Gets the currently active notes in the synth as an array of midi note numbers
	/// </summary>
	void GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const;

private:

	PlaybackFrame* _frame;
	PlaybackParameters* _streamParameters;
	Synth* _synth;
	Accumulator<TSignal>** _output;
	bool _initialized;
};


template<SignalValue TSignal>
SynthPlaybackDevice<TSignal>::SynthPlaybackDevice()
{
	_frame = nullptr;
	_streamParameters = nullptr;
	_synth = nullptr;
	_initialized = false;
	_output = nullptr;
}

template<SignalValue TSignal>
SynthPlaybackDevice<TSignal>::~SynthPlaybackDevice()
{
	delete _streamParameters;
	delete _synth;
	delete _frame;
	
	for (int index = 0; index < _streamParameters->GetNumberOfChannels(); index++)
	{
		delete _output[index];
	}

	delete[] _output;
}

template<SignalValue TSignal>
bool SynthPlaybackDevice<TSignal>::Initialize(const SynthConfiguration* configuration, const PlaybackParameters& parameters)
{
	_synth = new Synth(configuration, parameters.GetNumberOfChannels(), parameters.GetSamplingRate());
	_frame = new PlaybackFrame(parameters.GetNumberOfChannels());
	_streamParameters = new PlaybackParameters(parameters);
	_output = new Accumulator<TSignal>*[parameters.GetNumberOfChannels()];

	for (int index = 0; index < parameters.GetNumberOfChannels(); index++)
	{
		_output[index] = new Accumulator<TSignal>();
	}

	_initialized = true;

	return _initialized;
}

template<SignalValue TSignal>
int SynthPlaybackDevice<TSignal>::WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime)
{
	if (!_initialized)
		return -1;

	TSignal* outputBuffer = (TSignal*)playbackBuffer;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int frameIndex = 0; frameIndex < numberOfFrames; frameIndex++)
	{
		double absoluteTime = streamTime + (frameIndex / (double)_streamParameters->GetSamplingRate());

		// Clear Frame
		_frame->Clear();

		// Get Samples for N channels
		_synth->GetSample(_frame, absoluteTime);

		// Interleved frames
		for (unsigned int channelIndex = 0; channelIndex < _streamParameters->GetNumberOfChannels(); channelIndex++)
		{
			// Set output sample
			outputBuffer[(2 * frameIndex) + channelIndex] = _frame->GetSample(channelIndex);

			// Keep accumulator value for output UI
			_output[channelIndex]->Add(fabsf(_frame->GetSample(channelIndex)));
		}
	}

	return 0;
}
//
//template<SignalValue TSignal>
//int SynthPlaybackDevice<TSignal>::WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackBuffer, unsigned int numberOfFrames, double streamTime)
//{
//	if (!_initialized)
//		return -1;
//
//	//hasOutput = false;
//
//	//// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
//	//for (unsigned int frameIndex = 0; frameIndex < numberOfFrames; frameIndex++)
//	//{
//	//	double absoluteTime = streamTime + (frameIndex / (double)playbackBuffer->GetSamplingRate());
//
//	//	// Interleved frames
//	//	for (unsigned int channelIndex = 0; channelIndex < playbackBuffer->GetNumberOfChannels(); channelIndex++)
//	//	{
//	//		// Initialize sample to zero
//	//		TSignal sample = (TSignal)_synth->GetSample(absoluteTime);
//
//	//		// Set output sample
//	//		playbackBuffer->SetBufferFrame(sample, frameIndex, channelIndex);
//	//	}
//	//}
//
//	return 0;
//}

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
void SynthPlaybackDevice<TSignal>::UpdateSynth(const SynthConfiguration* configuration)
{
	_synth->SetConfiguration(configuration);
}

template<SignalValue TSignal>
TSignal SynthPlaybackDevice<TSignal>::GetOutput(int channelIndex) const
{
	return _output[channelIndex]->GetAvg();
}

template<SignalValue TSignal>
void SynthPlaybackDevice<TSignal>::GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const
{
	_synth->GetNotes(array, arrayLength);
}