#pragma once

#include "Accumulator.h"
#include "MidiEvent.h"
#include "MidiEventList.h"
#include "MidiFile.h"
#include "PlaybackBuffer.h"
#include "PlaybackDevice.h"
#include "PlaybackFrame.h"
#include "PlaybackParameters.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include <cmath>
#include <functional>
#include <string>

template<SignalValue TSignal>
class MidiPlaybackDevice : public PlaybackDevice<TSignal>
{
public:

	MidiPlaybackDevice();
	~MidiPlaybackDevice();

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) override;
	bool GetLastOutput() const override;
	bool SetForPlayback(unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration) override;
	int WritePlaybackBuffer(
		TSignal* playbackBuffer,
		unsigned int numberOfFrames,
		double streamTime,
		const SynthConfiguration* configuration) override;

	/// <summary>
	/// Loads midi file and creates playback configuration
	/// </summary>
	bool Load(const std::string& fileName);

	/// <summary>
	/// Returns average output for specified channel from the last frame buffer write
	/// </summary>
	TSignal GetOutput(int channelIndex) const;

private:

	/// <summary>
	/// Callback function to aid in repetative midi iteration. The current time is the stream time + any
	/// other midi-related time precalculated for the midi event. 
	/// </summary>
	using MidiSynthEventCallback = std::function<void(double currentTime, const smf::MidiEvent& midiEvent)>;

	/// <summary>
	/// Sets synth for midi playback for the given frame
	/// </summary>
	/// <param name="currentStreamTime">Current stream time for the current frame</param>
	/// <param name="currentFrameIndex">Current frame index for writing the buffer</param>
	/// <param name="configuration">Synth configuration for playback</param>
	void SetMidiSynth(double currentStreamTime, int currentFrameIndex, const SynthConfiguration* configuration);

	/// <summary>
	/// Iterator for midi event stream
	/// </summary>
	/// <param name="currentStreamTime">Current stream time for the current frame</param>
	/// <param name="currentFrameIndex">Current frame index for writing the buffer</param>
	/// <param name="callback">Midi event callback that will contain the midi event for the specified time</param>
	void IterateMidiStream(double currentStreamTime, int currentFrameIndex, MidiSynthEventCallback callback);

private:

	smf::MidiFile* _midiFile;
	double _midiSecondsPerTick;					// This is in terms of MIDI ticks (using smf to calculate these during load)
	double _streamStartTime;

	Synth* _synth;
	PlaybackFrame* _frame;
	Accumulator<TSignal>** _output;
	unsigned int _numberOfChannels;
	unsigned int _samplingRate;

	bool _isLoaded;
	bool _lastOutput;
	bool _initialized;
};

template<SignalValue TSignal>
MidiPlaybackDevice<TSignal>::MidiPlaybackDevice()
{
	_midiFile = nullptr;
	_midiSecondsPerTick = 0;
	_streamStartTime = 0;
	_lastOutput = false;	
	_numberOfChannels = 0;
	_samplingRate = 0;
	_frame = nullptr;
	_synth = nullptr;
	_isLoaded = false;
	_initialized = false;
	_output = nullptr;
}

template<SignalValue TSignal>
MidiPlaybackDevice<TSignal>::~MidiPlaybackDevice()
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
bool MidiPlaybackDevice<TSignal>::Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters)
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
bool MidiPlaybackDevice<TSignal>::SetForPlayback(unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration)
{
	bool newPlayback = false;

	for (int currentFrameIndex = 0; currentFrameIndex < numberOfFrames; currentFrameIndex++)
	{
		// Current Stream Time
		double currentStreamTime = streamTime + (currentFrameIndex / (double)_samplingRate);

		this->IterateMidiStream(currentStreamTime, currentFrameIndex, [&newPlayback](double currentTime, const smf::MidiEvent& midiEvent) {

			// Note On
			if (midiEvent.isNoteOn())
			{
				newPlayback = true;
				return;
			}
		});
	}

	return newPlayback;
}

template<SignalValue TSignal>
void MidiPlaybackDevice<TSignal>::SetMidiSynth(double currentStreamTime, int currentFrameIndex, const SynthConfiguration* configuration)
{
	Synth* synth = _synth;

	this->IterateMidiStream(currentStreamTime, currentFrameIndex, [&synth, &configuration](double currentTime, const smf::MidiEvent& midiEvent) {

		// Note On
		if (midiEvent.isNoteOn())
		{
			synth->Set(midiEvent.getKeyNumber(), true, currentTime, configuration);
		}

		// Note Off
		else if (midiEvent.isNoteOff())
		{
			synth->Set(midiEvent.getKeyNumber(), false, currentTime, configuration);
		}
	});
}

template<SignalValue TSignal>
int MidiPlaybackDevice<TSignal>::WritePlaybackBuffer(
	TSignal* playbackBuffer,
	unsigned int numberOfFrames, 
	double streamTime, 
	const SynthConfiguration* configuration)
{
	if (!_initialized)
		return -1;

	// Procedure
	//
	// 0) Update synth configuration
	// 1) Calculate the next batch of midi events; and which frames match the 
	//	  current frame index.
	// 2) Align the midi / stream times
	// 
	// Produce the playback buffer from the synth in a loop over the stream's 
	// frame index, looking for MIDI events that are in the file.
	//

	TSignal* outputBuffer = (TSignal*)playbackBuffer;

	// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
	for (unsigned int frameIndex = 0; frameIndex < numberOfFrames; frameIndex++)
	{
		double absoluteTime = streamTime + (frameIndex / (double)_samplingRate);

		// Set Synth Midi Events
		this->SetMidiSynth(absoluteTime, frameIndex, configuration);

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
void MidiPlaybackDevice<TSignal>::IterateMidiStream(double currentStreamTime, int currentFrameIndex, MidiSynthEventCallback callback)
{
	// This will calculate the delta-time involved with the previous frame call
	double lastStreamTime = currentFrameIndex == 0 ? currentStreamTime : (currentStreamTime + ((currentFrameIndex - 1) / (double)_samplingRate));

	int startTick = lastStreamTime / _midiSecondsPerTick;
	int endTick = currentStreamTime / _midiSecondsPerTick;

	for (int tick = startTick; tick <= endTick; tick++)
	{
		// Current time in terms of the playback stream
		float currentTime = currentStreamTime + (_midiSecondsPerTick * (tick - startTick));

		for (int trackIndex = 0; trackIndex < _midiFile->getTrackCount(); trackIndex++)
		{
			if (tick >= _midiFile[trackIndex].size())
				continue;

			smf::MidiEventList midiEvents = _midiFile[trackIndex][tick];

			for (int midiEventIndex = 0; midiEventIndex < midiEvents.size(); midiEventIndex++)
			{
				// Callback:  Current Stream Time, Midi Event
				callback(currentTime, midiEvents[midiEventIndex]);
			}
		}
	}
}

template<SignalValue TSignal>
bool MidiPlaybackDevice<TSignal>::Load(const std::string& fileName)
{
	_midiFile = new smf::MidiFile(fileName);

	// Set "absolute ticks mode"
	_midiFile->absoluteTicks();

	// Finally, try merging tracks to create a single-track song
	// midiFile.joinTracks();

	_midiSecondsPerTick = _midiFile->getFileDurationInSeconds() / _midiFile->getFileDurationInTicks();
	_isLoaded = true;

	return _isLoaded;
}

template<SignalValue TSignal>
TSignal MidiPlaybackDevice<TSignal>::GetOutput(int channelIndex) const
{
	return TSignal();
}

template<SignalValue TSignal>
bool MidiPlaybackDevice<TSignal>::GetLastOutput() const
{
	return _lastOutput;
}