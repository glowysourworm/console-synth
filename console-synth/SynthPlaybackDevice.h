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

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) override;

	int WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration) override;
	//int WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackBuffer, unsigned int numberOfFrames, double streamTime) override;

	void SetNote(int midiNumber, bool pressed, double streamTime, const SynthConfiguration* configuration);
	void UpdateSynth(const SynthConfiguration* configuration);

	/// <summary>
	/// Returns average output for specified channel from the last frame buffer write
	/// </summary>
	TSignal GetOutput(int channelIndex) const;

	/// <summary>
	/// Returns true if the synth had output last call to WritePlaybackBuffer. This output means there's more in the 
	/// synth device to write until there's nothing left; and the output buffer can write silence, which saves CPU
	/// cycles.
	/// </summary>
	bool GetLastOutput() const;

public:

	/// <summary>
	/// Gets the currently active notes in the synth as an array of midi note numbers
	/// </summary>
	void GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const;

private:

	PlaybackFrame* _frame;
	Synth* _synth;
	Accumulator<TSignal>** _output;
	unsigned int _numberOfChannels;
	unsigned int _samplingRate;
	bool _lastOutput;
	bool _initialized;
};


template<SignalValue TSignal>
SynthPlaybackDevice<TSignal>::SynthPlaybackDevice()
{
	_lastOutput = false;
	_numberOfChannels = 0;
	_samplingRate = 0;
	_frame = nullptr;
	_synth = nullptr;
	_initialized = false;
	_output = nullptr;
}

template<SignalValue TSignal>
SynthPlaybackDevice<TSignal>::~SynthPlaybackDevice()
{
	delete _synth;
	delete _frame;
	
	for (int index = 0; index < _numberOfChannels; index++)
	{
		delete _output[index];
	}

	delete[] _output;
}

template<SignalValue TSignal>
bool SynthPlaybackDevice<TSignal>::Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters)
{
	_numberOfChannels = parameters->GetNumberOfChannels();
	_samplingRate = parameters->GetSamplingRate();

	_synth = new Synth(configuration, _numberOfChannels, _samplingRate);
	_frame = new PlaybackFrame(_numberOfChannels);
	_output = new Accumulator<TSignal>*[_numberOfChannels];

	for (int index = 0; index < _numberOfChannels; index++)
	{
		_output[index] = new Accumulator<TSignal>();
	}

	_initialized = true;

	return _initialized;
}

template<SignalValue TSignal>
int SynthPlaybackDevice<TSignal>::WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration)
{
	if (!_initialized)
		return -1;

	TSignal* outputBuffer = (TSignal*)playbackBuffer;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int frameIndex = 0; frameIndex < numberOfFrames; frameIndex++)
	{
		double absoluteTime = streamTime + (frameIndex / (double)_samplingRate);

		// Clear Frame
		_frame->Clear();

		// Get Samples for N channels
		_lastOutput = _synth->GetSample(_frame, absoluteTime, configuration);

		// Interleved frames
		for (unsigned int channelIndex = 0; channelIndex < _numberOfChannels; channelIndex++)
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
void SynthPlaybackDevice<TSignal>::SetNote(int midiNumber, bool pressed, double streamTime, const SynthConfiguration* configuration)
{
	_synth->Set(midiNumber, pressed, streamTime, configuration);
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
inline bool SynthPlaybackDevice<TSignal>::GetLastOutput() const
{
	return _lastOutput;
}

template<SignalValue TSignal>
void SynthPlaybackDevice<TSignal>::GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const
{
	_synth->GetNotes(array, arrayLength);
}