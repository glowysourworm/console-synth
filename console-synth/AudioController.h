#pragma once

#include "BaseController.h"
#include "IntervalTimer.h"
#include "LoopTimer.h"
#include "MidiPlaybackDevice.h"
#include "PlaybackClock.h"
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"
#include <atomic>
#include <string>

class AudioController : public BaseController
{
public:

	AudioController();
	~AudioController();

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) override;
	bool Dispose() override;

	/// <summary>
	/// Switches between midi / regular mode
	/// a file to load. 
	/// </summary>
	void SetMidiMode(const std::string& midiFile);

	/// <summary>
	/// Switches between midi / synth mode
	/// </summary>
	void SetSynthMode();

	/// <summary>
	/// Simple output function (this will be replaced with an output processing stage)
	/// </summary>
	void GetUpdate(float& streamTime, float& audioTime, float& frontendTime, float& latency, float& left, float& right);

public:

	/// <summary>
	/// Processes audio from the RT Audio backend - which fills the output buffer with the provided
	/// number of samples (interleved frames).
	/// </summary>
	/// <param name="outputBuffer">RT Audio buffer</param>
	/// <param name="numberOfFrames">Number of (L/R) frames to process</param>
	/// <param name="streamTime">Current stream time from RT Audio</param>
	/// <param name="configuration">This should be the SynthConfiguration* which is provided on this thread to process, also.</param>
	int ProcessAudioCallback(float* outputBuffer, unsigned int numberOfFrames, double streamTime, SynthConfiguration* configuration);

private:

	/// <summary>
	/// Sets synth device for new pressed keys. Returns true if there are keys that are pressed.
	/// </summary>
	bool ProcessKeyStrokes(double streamTime, SynthConfiguration* configuration);

private:

	std::atomic<bool> _blockCallback;
	bool _midiMode;	
	bool _initialized;

	SynthPlaybackDevice<float>* _synthDevice;
	MidiPlaybackDevice<float>* _midiDevice;

	PlaybackClock* _streamClock;
	LoopTimer* _audioTimer;
	IntervalTimer* _synthIntervalTimer;

	float _outputL;
	float _outputR;
};

