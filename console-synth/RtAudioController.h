#pragma once

#include "PlaybackParameters.h"
#include "RtAudio.h"
#include "SynthConfiguration.h"
#include <functional>
#include <string>

/// <summary>
/// Static functions for RT Audio callbacks; and the container for the RtAudio* instance
/// </summary>
class RtAudioController
{
public:


	/// <summary>
	/// Callback to process RT Audio data (which has a callback with extra variables we don't need)
	/// </summary>
	using AudioCallbackDelegate = std::function<int(float* outputBuffer, unsigned int numberFrames, double streamTime, SynthConfiguration* configuration)>;

	//using AudioCallbackDelegate = std::function<int(float*, unsigned int, void*)>;

	/// <summary>
	/// Primary RT Audio Callback:  They have a separate thread managing the device audio. So, this will be on their thread; and we 
	/// will process all of our SynthPlaybackDevice* work here - including key strokes. 
	/// </summary>
	/// <param name="outputBuffer">Output Audio Buffer (see RTAudio initialization)</param>
	/// <param name="inputBuffer">Input Audio (used for recording or duplex mode)</param>
	/// <param name="nFrames">Number of audio frames (Frame = { channel 1, .. channel N }, usually L/R channels)</param>
	/// <param name="streamTime">Stream time in seconds</param>
	/// <param name="status">RT Audio stream status</param>
	/// <param name="userData">This will contain a pointer to the synth configuration. However, it is not thread safe!</param>
	/// <returns>Error indicator to RT Audio</returns>
	static int AudioCallback(void* outputBuffer,
							 void* inputBuffer,
							 unsigned int nFrames,
							 double streamTime,
							 RtAudioStreamStatus status,
							 void* userData);

	/// <summary>
	/// Primary RT Audio Error Callback: This will be called on a separate thread when there is a backend error in
	/// RT Audio.
	/// </summary>
	/// <param name="type">Type of error</param>
	/// <param name="errorText">Error text</param>
	static void ErrorCallback(RtAudioErrorType type, const std::string& errorText);

	/// <summary>
	/// Initializes RT Audio:  Must be called before use. Once called, must call Dispose before re-calling.
	/// </summary>
	static bool Initialize(const AudioCallbackDelegate& callback);

	/// <summary>
	/// Disposes RT Audio:  Must be called to close / delete RT Audio + Stream
	/// </summary>
	static bool Dispose();

	/// <summary>
	/// Gets static instance of the playback parameters
	/// </summary>
	static PlaybackParameters* GetPlaybackParameters();

	/// <summary>
	/// Opens the RT Audio backend stream with specified user data
	/// </summary>
	/// <param name="userData">Arbitrary data sent along with the backend stream</param>
	static bool OpenStream(void* userData);
	static bool CloseStream();
	static bool IsStreamOpen();
	static bool IsStreamRunning();

	static PlaybackParameters* GetParameters()
	{
		return RtAudioController::Parameters;
	}

protected:

	static RtAudio* Instance;
	static RtAudio::DeviceInfo* OutputDevice;

	static RtAudioErrorType LastErrorType;
	static std::string* LastErrorText;

	static PlaybackParameters* Parameters;
	static AudioCallbackDelegate* Callback;

	static bool Initialized;

};