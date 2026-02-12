#pragma once

#include "Accumulator.h"
#include "Constant.h"
#include "PlaybackBuffer.h"
#include "PlaybackDevice.h"
#include "PlaybackFrame.h"
#include "PlaybackParameters.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include "WindowsKeyCodes.h"
#include <Windows.h>
#include <cmath>
#include <exception>

template<SignalValue TSignal>
class SynthPlaybackDevice : public PlaybackDevice<TSignal>
{
public:

	SynthPlaybackDevice();
	~SynthPlaybackDevice();

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) override;

	bool SetForPlayback(unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration) override;

	int WritePlaybackBuffer(
		TSignal* playbackBuffer,
		unsigned int numberOfFrames, 
		double streamTime, 
		const SynthConfiguration* configuration) override;

	bool GetLastOutput() const override;

	/// <summary>
	/// Returns average output for specified channel from the last frame buffer write
	/// </summary>
	TSignal GetOutput(int channelIndex) const;

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
bool SynthPlaybackDevice<TSignal>::SetForPlayback(unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration)
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	// Update synth configuration
	_synth->SetConfiguration(configuration);

	bool pressedKeys = false;

	// Iterate Key Codes (probably the most direct method)
	//
	for (int keyCode = (int)WindowsKeyCodes::NUMBER_0; keyCode <= (int)WindowsKeyCodes::PERIOD; keyCode++)
	{
		// Check that enum is defined
		if (keyCode < 0x30 ||
			keyCode == 0x40 ||
			(keyCode > 0x5A && keyCode < 0x80) ||
			(keyCode > 0x80 && keyCode < 0xBB) ||
			(keyCode > 0xBF && keyCode < 0xDB) ||
			(keyCode > 0xDE))
			continue;

		if (!configuration->HasMidiNote((WindowsKeyCodes)keyCode))
			continue;

		// Pressed
		bool isPressed = GetAsyncKeyState(keyCode) & 0x8000;

		// Midi Note
		int midiNote = configuration->GetMidiNote((WindowsKeyCodes)keyCode);

		// Engage / Dis-Engage
		_synth->Set(midiNote, isPressed, streamTime, configuration);

		pressedKeys |= isPressed;
	}

	return pressedKeys;
}

template<SignalValue TSignal>
int SynthPlaybackDevice<TSignal>::WritePlaybackBuffer(TSignal* playbackBuffer, unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration)
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

template<SignalValue TSignal>
TSignal SynthPlaybackDevice<TSignal>::GetOutput(int channelIndex) const
{
	return _output[channelIndex]->GetAvg();
}

template<SignalValue TSignal>
bool SynthPlaybackDevice<TSignal>::GetLastOutput() const
{
	return _lastOutput;
}
