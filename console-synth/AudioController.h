#pragma once

#include "IntervalTimer.h"
#include "LoopTimer.h"
#include "PlaybackClock.h"
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"

class AudioController
{
public:

	AudioController(const SynthConfiguration* configuration);
	~AudioController();

	bool Initialize();
	bool Dispose();

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

	void ProcessKeyStrokes(double streamTime);

private:

	bool _initialized;

	SynthPlaybackDevice<float>* _synthDevice;
	const SynthConfiguration* _configuration;

	PlaybackClock* _streamClock;
	LoopTimer* _audioTimer;
	IntervalTimer* _synthIntervalTimer;

	float _outputL;
	float _outputR;
};

