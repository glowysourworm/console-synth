#pragma once

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
	/// <param name="numberOfFrames">Number of frames in the stream. Each frame will have N number of channels (usually 2 for L / R)</param>
	/// <param name="numberOfChannels">Number of channels in the stream, usually 2 for L / R</param>
	/// <param name="streamTime">Absolute stream time, usually in milliseconds</param>
	/// <returns>Status code for RT Audio</returns>
	virtual int StreamCallback(void* outputBuffer,
		unsigned int numberOfFrames,
		unsigned int numberOfChannels,		
		double streamTime) = 0;
};

