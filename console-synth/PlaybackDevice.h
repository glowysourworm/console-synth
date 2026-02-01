#pragma once

#include "RtAudio.h"
class PlaybackDevice
{
public:

	PlaybackDevice();
	~PlaybackDevice();

	/// <summary>
	/// Initialization of playback device is required before starting RT Audio playback.
	/// </summary>
	/// <returns>Returns true if device is ready, otherwise false for some sort of error</returns>
	virtual bool Initialize() = 0;

	/// <summary>
	/// Primary callback for RT Audio! This will be called in real time to get a sample for your
	/// output device.
	/// </summary>
	/// <param name="outputBuffer">Interleved output frames for the stream</param>
	/// <param name="inputBuffer">TBD (see rt audio docs)</param>
	/// <param name="nFrames">Number of frames in the stream, usually 2 for L / R</param>
	/// <param name="streamTime">Absolute stream time, usually in milliseconds</param>
	/// <param name="status">RT Audio backend stream status</param>
	/// <param name="userData">Empty user data memory pointer</param>
	/// <returns>Status code for RT Audio</returns>
	virtual int RtAudioCallback(void* outputBuffer,
		void* inputBuffer,
		unsigned int nFrames,
		double streamTime,
		RtAudioStreamStatus status,
		void* userData) = 0;
};

