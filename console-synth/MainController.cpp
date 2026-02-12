#include "AudioController.h"
#include "LoopTimer.h"
#include "MainController.h"
#include "PlaybackParameters.h"
#include "RtAudioController.h"
#include "SynthConfiguration.h"
#include "UIController.h"
#include <chrono>
#include <functional>
#include <thread>

MainController::MainController()
{
	_configuration = nullptr;
	_uiTimer = new LoopTimer(0.075);
	_audioController = new AudioController();
	_uiController = new UIController();
}

MainController::~MainController()
{
	delete _configuration;
	delete _uiTimer;
	delete _audioController;
	delete _uiController;
}

/// <summary>
/// Initialization function for the synth backend. This must be called before starting the player!
/// </summary>
bool MainController::Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters)
{
	// (Constructor pattern...)
	_configuration = new SynthConfiguration(*configuration);

	// RT AUDIO
	bool success = RtAudioController::Initialize(std::bind(&AudioController::ProcessAudioCallback,
												_audioController,
												std::placeholders::_1,
												std::placeholders::_2,
												std::placeholders::_3,
												std::placeholders::_4));

	// RT AUDIO -> Open Stream (SynthConfiguration*)(PlaybackParameteres*) (INITIALIZE!)
	success &= RtAudioController::OpenStream(_configuration);

	// Audio Controller (for callback)
	success &= _audioController->Initialize(configuration, parameters);

	// UI Controller (stream must be open) (Starts Thread!)
	success &= _uiController->Initialize(configuration, parameters);

	return success;
}

bool MainController::Dispose()
{
	// Stops UI thread
	return _audioController->Dispose() && _uiController->Dispose();
}

void MainController::Loop()
{
	// Playback Parameters (to update)
	float streamTime, audioTime, frontendTime, latency, left, right;

	// Primary Loop!!! 
	//
	// Audio Thread:  The RT Audio callback will arrive on their background thread from the audio backend. We
	//				  just have to manage two shared pointers:  PlaybackParameters*, and SynthConfiguration*.
	//				  The SynthPlaybackDevice* will be updated, from the synth configuration, during the 
	//				  callback. So, this must be synchronized somehow. Probably a std::atomic which could be
	//				  shared inside the SynthConfiguration*
	// 
	// UI Thread:  Unfortunately, the UI had to be run on a separate thread; but that won't really matter. We
	//			   can interrupt the thread to post UI updates. We're going to keep track of the UI timer here
	//			   and interrupt it every ~10ms.
	// 
	// The exit condition should be polled from the UI thread to this thread; and we'll use an interruption to
	// give / send data while the loop runs.
	//
	while (true)
	{
		// We may use these on this thread since we're calling the audio controller from here. The
		// callback from RT Audio WILL NOT SET this pointer's data!
		//
		PlaybackParameters* playbackParameters = RtAudioController::GetPlaybackParameters();

		// Update Playback Parameters:
		//
		_audioController->GetUpdate(streamTime, audioTime, frontendTime, latency, left, right);
		
		playbackParameters->UpdateRT(streamTime, _uiTimer->GetAvgMilli(), audioTime, frontendTime, latency);

		// Synth Configuration:  Shared pointer with the RT Audio callback
		//
		if (_uiTimer->Mark())
		{
			// Update <- UI (If user has made UI changes, they get set in the SynthConfiguration*) (SHARED!)
			//
			//if (_uiController->IsDirty())
			//	_uiController->FromUI(_configuration);

			// Update -> UI (PlaybackParameters*) (NOT SHARED) (Member functions all go through this thread for playback parameters)
			_uiController->ToUI(playbackParameters);
		}

		// CRITICAL SECTION:  This is an update from the UI, which will reset the synth parameters. So,
		//					  it is only allowed every ~100ms at the most.
		//
		if (_configuration->IsDirty())
		{
			//_rtAudio->stopStream();

			//_synthDevice->UpdateSynth(_configuration);

			// This timer is not thread-safe; but the usage seems to be ok on both threads
			// without stopping the audio stream. 
			//
			_uiTimer->Reset();

			//// Reset Configuration Flag
			//_configuration->ClearDirty();

			//_rtAudio->startStream();
		}

		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}
