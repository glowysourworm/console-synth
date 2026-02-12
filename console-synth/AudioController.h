#pragma once

#include "BaseController.h"
#include "IntervalTimer.h"
#include "LoopTimer.h"
#include "PlaybackClock.h"
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"

class AudioController : public BaseController
{
public:

	AudioController();
	~AudioController();

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) override;
	bool Dispose() override;

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

	void ProcessKeyStrokes(double streamTime, SynthConfiguration* configuration);

private:

	bool _initialized;

	SynthPlaybackDevice<float>* _synthDevice;

	PlaybackClock* _streamClock;
	LoopTimer* _audioTimer;
	IntervalTimer* _synthIntervalTimer;

	float _outputL;
	float _outputR;
};

