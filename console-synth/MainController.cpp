#include "AudioController.h"
#include "LoopTimer.h"
#include "MainController.h"
#include "PlaybackParameters.h"
#include "RtAudioController.h"
#include "SynthConfiguration.h"
#include "UIController.h"
#include "WindowsKeyCodes.h"
#include <chrono>
#include <functional>
#include <thread>

MainController::MainController()
{
	_configuration = new SynthConfiguration();
	_uiTimer = new LoopTimer(0.075);

	// Octave 1
	_configuration->SetMidiNote(WindowsKeyCodes::Z, 21);
	_configuration->SetMidiNote(WindowsKeyCodes::X, 22);
	_configuration->SetMidiNote(WindowsKeyCodes::C, 23);
	_configuration->SetMidiNote(WindowsKeyCodes::V, 24);
	_configuration->SetMidiNote(WindowsKeyCodes::B, 25);
	_configuration->SetMidiNote(WindowsKeyCodes::N, 26);
	_configuration->SetMidiNote(WindowsKeyCodes::M, 27);
	_configuration->SetMidiNote(WindowsKeyCodes::COMMA, 28);
	_configuration->SetMidiNote(WindowsKeyCodes::PERIOD, 29);
	_configuration->SetMidiNote(WindowsKeyCodes::QUESTION_MARK, 30);

	// Octave 2
	_configuration->SetMidiNote(WindowsKeyCodes::A, 33);
	_configuration->SetMidiNote(WindowsKeyCodes::S, 34);
	_configuration->SetMidiNote(WindowsKeyCodes::D, 35);
	_configuration->SetMidiNote(WindowsKeyCodes::F, 36);
	_configuration->SetMidiNote(WindowsKeyCodes::G, 37);
	_configuration->SetMidiNote(WindowsKeyCodes::H, 38);
	_configuration->SetMidiNote(WindowsKeyCodes::J, 39);
	_configuration->SetMidiNote(WindowsKeyCodes::K, 40);
	_configuration->SetMidiNote(WindowsKeyCodes::L, 41);
	_configuration->SetMidiNote(WindowsKeyCodes::SEMICOLON, 42);
	_configuration->SetMidiNote(WindowsKeyCodes::APOSTROPHE, 43);

	// Octave 3
	_configuration->SetMidiNote(WindowsKeyCodes::Q, 44);
	_configuration->SetMidiNote(WindowsKeyCodes::W, 45);
	_configuration->SetMidiNote(WindowsKeyCodes::E, 46);
	_configuration->SetMidiNote(WindowsKeyCodes::R, 47);
	_configuration->SetMidiNote(WindowsKeyCodes::T, 48);
	_configuration->SetMidiNote(WindowsKeyCodes::Y, 49);
	_configuration->SetMidiNote(WindowsKeyCodes::U, 50);
	_configuration->SetMidiNote(WindowsKeyCodes::I, 51);
	_configuration->SetMidiNote(WindowsKeyCodes::O, 52);
	_configuration->SetMidiNote(WindowsKeyCodes::P, 53);
	_configuration->SetMidiNote(WindowsKeyCodes::LEFT_BRACKET, 54);
	_configuration->SetMidiNote(WindowsKeyCodes::RIGHT_BRACKET, 55);

	// Octave 4
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_1, 56);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_2, 57);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_3, 58);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_4, 59);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_5, 60);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_6, 61);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_7, 62);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_8, 63);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_9, 64);
	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_0, 65);
	_configuration->SetMidiNote(WindowsKeyCodes::MINUS, 66);
	_configuration->SetMidiNote(WindowsKeyCodes::PLUS, 67);

	_audioController = new AudioController(_configuration);
	_uiController = new UIController(_configuration);
}

MainController::~MainController()
{
	delete _configuration;
	delete _uiTimer;
}

/// <summary>
/// Initialization function for the synth backend. This must be called before starting the player!
/// </summary>
bool MainController::Initialize()
{
	// RT AUDIO
	bool success = RtAudioController::Initialize(std::bind(&AudioController::ProcessAudioCallback,
												_audioController,
												std::placeholders::_1,
												std::placeholders::_2,
												std::placeholders::_3,
												std::placeholders::_4));

	// Audio Controller (for callback)
	success &= _audioController->Initialize();

	// RT AUDIO -> Open Stream (SynthConfiguration*)
	success &= RtAudioController::OpenStream(_configuration);

	// UI Controller (stream must be open) (Starts Thread!)
	_uiController->Initialize(RtAudioController::GetPlaybackParameters());

	return success;
}

bool MainController::Dispose()
{
	bool success = _audioController->Dispose();

	// Stops Thread!
	_uiController->Dispose();

	return success;
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
		
		playbackParameters->UpdateRTParameters(streamTime, _uiTimer->GetAvgMilli(), audioTime, frontendTime, latency);

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
