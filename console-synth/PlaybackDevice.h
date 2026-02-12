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
	/// Tells the playback device to write its output to the playback buffer. The write should start at frame index 0, and
	/// end at the frame index that coincides with the stream end time. The function should return non-zero for error indication.
	/// </summary>
	virtual int WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime) = 0;

	/// <summary>
	/// Tells the playback device to write its output to the playback buffer. The write should start at frame index 0, and
	/// end at the frame index that coincides with the stream end time. The function should return non-zero for error indication.
	/// </summary>
	//virtual int WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackBuffer, unsigned int numberOfFrames, double streamTime) = 0;
};


template<SignalValue TSignal>
PlaybackDevice<TSignal>::PlaybackDevice()
{
}

template<SignalValue TSignal>
PlaybackDevice<TSignal>::~PlaybackDevice()
{
}