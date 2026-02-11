#include "PlaybackParameters.h"
#include "RtAudioController.h"
#include "SynthConfiguration.h"
#include <RtAudio.h>
#include <exception>
#include <functional>
#include <string>

// These are required for the compiler (for the C++ linker)
//
// ---------------- Static Initializer -------------------
RtAudio* RtAudioController::Instance;
RtAudio::DeviceInfo* RtAudioController::OutputDevice;

RtAudioErrorType RtAudioController::LastErrorType;
std::string* RtAudioController::LastErrorText;

PlaybackParameters* RtAudioController::Parameters;
RtAudioController::AudioCallbackDelegate* RtAudioController::Callback;

bool RtAudioController::Initialized;
// -------------------------------------------------------

int RtAudioController::AudioCallback(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData)
{
	if (!RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller not initialzed! Backend thread still running!");

	if (!RtAudioController::IsStreamOpen())
		throw new std::exception("RT Audio Controller stream not open! Backend thread still running!");

	// Audio Callback:  Casting (void*) user data to our synth configuration! And, the output buffer!
	//
    return (*RtAudioController::Callback)((float*)outputBuffer, nFrames, streamTime, (SynthConfiguration*)userData);
}

void RtAudioController::ErrorCallback(RtAudioErrorType type, const std::string& errorText)
{
	if (!RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller not initialzed! Backend thread still running!");

	if (!RtAudioController::IsStreamOpen())
		throw new std::exception("RT Audio Controller stream not open! Backend thread still running!");

	RtAudioController::LastErrorText->clear();
	RtAudioController::LastErrorText->append(errorText);
	RtAudioController::LastErrorType = type;
}

bool RtAudioController::Initialize(const AudioCallbackDelegate& callback)
{
	if (RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller already initialzed! Must call Dispose() before re-initializing the backend");

	try
	{
		// Buffer Frame Calculation:  How many buffer frames will be appropriate?
		//
		// After some tedious work with the Windows WASAPI, and using our real time
		// loop, it was obvious that there is a difficult problem trying to synchronize
		// the audio backend with the frontend looping. 
		//
		// The approximate time for our primary loop is ~7ms. Rendering the actual 
		// audio is a very small portion of this time, for synchronous setup.
		//
		// RT Audio callbacks are on a separate thread. So, we're going to try to
		// manage our loop so the time to render audio is small. Also, there will
		// be a shared pointer to the SynthConfiguration*, which will be carefully
		// set by our thread, and read by the other. This is NOT a thread-safe 
		// operation; but we won't care as long as we are just setting primitive 
		// variables.
		//
		// The size of the RT Audio buffer should be set from the device. The WASAPI
		// returned approx ~10.6667ms as the default period. So, we're going to select
		// an audio buffer time of this much; and see how that works. Later, we'll 
		// try to take this number from the device itself.
		//

		RtAudioController::Instance = new RtAudio(RtAudio::Api::WINDOWS_WASAPI, &RtAudioController::ErrorCallback);
		RtAudioController::Callback = new AudioCallbackDelegate(callback);

		RtAudio::StreamParameters outputParameters;

		// Output Device
		//
		auto outputDeviceIndex = RtAudioController::Instance->getDefaultOutputDevice();
		auto outputDevice = RtAudioController::Instance->getDeviceInfo(outputDeviceIndex);

		outputParameters.deviceId = outputDevice.ID;
		outputParameters.nChannels = outputDevice.outputChannels;
		outputParameters.firstChannel = 0;

		RtAudio::StreamOptions options;

		//options.flags |= RTAUDIO_SCHEDULE_REALTIME;					
		options.numberOfBuffers = 4;						// Has to do with audio format!
		options.flags |= RTAUDIO_HOG_DEVICE;
		options.flags |= RTAUDIO_MINIMIZE_LATENCY;
		// options.flags |= RTAUDIO_NONINTERLEAVED; 


		// Output Buffer Calculation: ~device period (ms) * (s / ms) * (samples / s) = [samples]
		//
		// RT Audio:  (see openStream comments) will try to calculate a desired buffer size based on this input
		//			  value. So, we'll send it something the device likes; and see what it comes back with.
		//
		//unsigned int outputBufferFrameSize = (unsigned int)(10.6667 * 0.001 * outputDevice.preferredSampleRate);
		unsigned int outputBufferFrameSize = 16;
		unsigned int frontendFrameSize = outputBufferFrameSize;

		// Initialized
		RtAudioController::Initialized = true;
	}
	catch (std::exception ex)
	{
		// Initialize Failure
		RtAudioController::Initialized = true;
	}

	return RtAudioController::Initialized;
}

bool RtAudioController::Dispose()
{
	if (!RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller not initialzed! Must call Initialize() before disposing the stream");

	try
	{
		if (RtAudioController::Parameters != nullptr)
		{
			delete RtAudioController::Parameters;
			RtAudioController::Parameters = nullptr;
		}
		if (RtAudioController::OutputDevice != nullptr)
		{
			delete RtAudioController::OutputDevice;
			RtAudioController::OutputDevice = nullptr;
		}

		if (RtAudioController::Instance->isStreamRunning())
			RtAudioController::Instance->stopStream();

		if (RtAudioController::Instance->isStreamOpen())
			RtAudioController::Instance->closeStream();
		
		// RESET 
		delete RtAudioController::Instance;
		delete RtAudioController::Callback;

		RtAudioController::Instance = nullptr;
		RtAudioController::Callback = nullptr;

		// Must Re-Initialize!
		RtAudioController::Initialized = false;

		return true;
	}
	catch (std::exception ex)
	{
		return false;
	}
}

PlaybackParameters* RtAudioController::GetPlaybackParameters()
{
	if (!RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller not initialzed! Must call Initialize() before opening the stream");

	if (!RtAudioController::Instance->isStreamOpen())
		throw new std::exception("RT Audio Controller stream not open! Must call OpenStream() to open the stream");

	return RtAudioController::Parameters;
}

bool RtAudioController::OpenStream(void* userData)
{
	if (!RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller not initialzed! Must call Initialize() before opening the stream");

	try
	{
		RtAudio::StreamParameters outputParameters;

		// Output Device
		//
		auto outputDeviceIndex = RtAudioController::Instance->getDefaultOutputDevice();
		auto outputDevice = RtAudioController::Instance->getDeviceInfo(outputDeviceIndex);
		RtAudioController::OutputDevice = new RtAudio::DeviceInfo(outputDevice);
		

		outputParameters.deviceId = outputDevice.ID;
		outputParameters.nChannels = outputDevice.outputChannels;
		outputParameters.firstChannel = 0;

		RtAudio::StreamOptions options;

		//options.flags |= RTAUDIO_SCHEDULE_REALTIME;					
		options.numberOfBuffers = 4;						// Has to do with audio format!
		options.flags |= RTAUDIO_HOG_DEVICE;
		options.flags |= RTAUDIO_MINIMIZE_LATENCY;
		// options.flags |= RTAUDIO_NONINTERLEAVED; 


		// Output Buffer Calculation: ~device period (ms) * (s / ms) * (samples / s) = [samples]
		//
		// RT Audio:  (see openStream comments) will try to calculate a desired buffer size based on this input
		//			  value. So, we'll send it something the device likes; and see what it comes back with.
		//
		//unsigned int outputBufferFrameSize = (unsigned int)(10.6667 * 0.001 * outputDevice.preferredSampleRate);
		unsigned int outputBufferFrameSize = 16;
		unsigned int frontendFrameSize = outputBufferFrameSize;

		RtAudioController::Instance->openStream(&outputParameters,	// 
			NULL,									// Duplex Mode (input parameters)
			RTAUDIO_FLOAT32,						// RT Audio Format
			outputDevice.preferredSampleRate,		// Device Sampling Rate
			&outputBufferFrameSize,					// Device (preferred) Frame Size (RT Audio will adjust this)
			&RtAudioController::AudioCallback,		// Audio Callback
			userData,								
			&options);

		auto hostApi = RtAudioController::Instance->getCurrentApi();

		const char* deviceFormat = "Bizzare Unknown";

		// Store output parameters
		RtAudioController::Parameters = new PlaybackParameters(
			RtAudioController::Instance->getApiDisplayName(hostApi).c_str(),
			deviceFormat,
			outputDevice.name.c_str(),
			outputDevice.preferredSampleRate,
			outputDevice.outputChannels,
			outputBufferFrameSize);

		// Start Stream!
		RtAudioController::Instance->startStream();
	}
	catch (std::exception ex)
	{
		return false;
	}

	return true;
}

bool RtAudioController::CloseStream()
{
	if (!RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller not initialzed! Must call Initialize() before opening the stream");

	if (!RtAudioController::Instance->isStreamOpen())
		throw new std::exception("RT Audio Controller stream not open! Must call OpenStream() to open the stream");

	try
	{
		RtAudioController::Instance->closeStream();

		return true;
	}
	catch (std::exception ex)
	{
		return false;
	}
}

bool RtAudioController::IsStreamOpen()
{
	if (!RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller not initialzed! Must call Initialize() before opening the stream");

	return RtAudioController::Instance->isStreamOpen();
}

bool RtAudioController::IsStreamRunning()
{
	if (!RtAudioController::Initialized)
		throw new std::exception("RT Audio Controller not initialzed! Must call Initialize() before opening the stream");

	return RtAudioController::Instance->isStreamRunning();
}
