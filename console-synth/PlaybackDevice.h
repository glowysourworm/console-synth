#pragma once

#include "PlaybackBuffer.h"
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"

template<SignalValue TSignal>
class PlaybackDevice
{
public:

	PlaybackDevice();
	~PlaybackDevice();

	/// <summary>
	/// Initialization of playback device is required before starting RT Audio playback.
	/// </summary>
	/// <returns>Returns true if device is ready, otherwise false for some sort of error</returns>
	virtual bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) = 0;

	/// <summary>
	/// Returns true if the synth had output last call to WritePlaybackBuffer. This output means there's more in the 
	/// synth device to write until there's nothing left; and the output buffer can write silence, which saves CPU
	/// cycles.
	/// </summary>
	virtual bool GetLastOutput() const = 0;

	/// <summary>
	/// Sets the playback device for this stream time prior to writing playback buffer. Returns
	/// true if there are samples to process for this playback period.
	/// </summary>
	/// <param name="numberOfFrames">Number of frames to process during next callback</param>
	/// <param name="streamTime">Current stream time to begin next playback period</param>
	/// <param name="configuration">Configuration for the synth device</param>
	/// <returns>True if there are samples to process. Otherwise, there should be silence for the next playback period</returns>
	virtual bool SetForPlayback(unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration) = 0;

	/// <summary>
	/// Tells the playback device to write its output to the playback buffer. The write should start at frame index 0, and
	/// end at the frame index that coincides with the stream end time. The function should return non-zero for error indication.
	/// </summary>
	virtual int WritePlaybackBuffer(TSignal* playbackBuffer, unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration) = 0;
};


template<SignalValue TSignal>
PlaybackDevice<TSignal>::PlaybackDevice()
{
}

template<SignalValue TSignal>
PlaybackDevice<TSignal>::~PlaybackDevice()
{
}