#include "AudioController.h"
#include "IntervalTimer.h"
#include "LoopTimer.h"
#include "PlaybackClock.h"
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"
#include "WindowsKeyCodes.h"
#include <Windows.h>
#include <exception>

AudioController::AudioController()
{
	_initialized = false;

	_synthDevice = new SynthPlaybackDevice<float>();
	_audioTimer = new LoopTimer(0.001);
	_streamClock = new PlaybackClock();
	_synthIntervalTimer = new IntervalTimer();

	_outputL = 0;
	_outputR = 0;
}

AudioController::~AudioController()
{
	if (_initialized)
		this->Dispose();
}

bool AudioController::Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters)
{
	if (_initialized)
		throw new std::exception("Audio Controller already initialized!");

	_synthDevice->Initialize(configuration, parameters);

	_streamClock->Reset();
	_streamClock->Start();
	_audioTimer->Reset();

	_initialized = true;

	return true;
}

int AudioController::ProcessAudioCallback(float* outputBuffer, unsigned int numberOfFrames, double streamTime, SynthConfiguration* configuration)
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	// Full Audio Loop Timer
	_audioTimer->Mark();

	// Frontend Processing Time (Start!)
	_synthIntervalTimer->Reset();

	// std::atomic wait loop
	while (true) {
		if (!configuration->IsWaiting())
		{
			if (configuration->SetWait(true))
				break;
		}
	}

	// Windows API, SynthConfiguration*, SynthPlaybackDevice* (be aware of usage)
	//
	this->ProcessKeyStrokes(streamTime, configuration);

	// Update SynthPlaybackDevice* (using shared configuration)
	_synthDevice->UpdateSynth(configuration);

	// Write playback buffer from synth device
	int returnValue = _synthDevice->WritePlaybackBuffer((void*)outputBuffer, numberOfFrames, streamTime);

	// Get output for the UI
	_outputL = _synthDevice->GetOutput(0);
	_outputR = _synthDevice->GetOutput(1);

	// std::atomic end loop (this should only run once!)
	while (!configuration->SetWait(false)) {}

	// Frontend Processing Time (Mark.)
	_synthIntervalTimer->Mark();

	return returnValue;
}

void AudioController::ProcessKeyStrokes(double streamTime, SynthConfiguration* configuration)
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	// Iterate Key Codes (probably the most direct method)
	//
	for (int keyCode = (int)WindowsKeyCodes::NUMBER_0; keyCode <= (int)WindowsKeyCodes::PERIOD; keyCode++)
	{
		// Check that enum is defined
		if (keyCode < 0x30 ||
			keyCode == 0x40 ||
			(keyCode > 0x5A && keyCode < 0x80) ||
			(keyCode > 0x80 && keyCode < 0xBB) ||
			(keyCode > 0xBF && keyCode < 0xDB) ||
			(keyCode > 0xDE))
			continue;

		if (!configuration->HasMidiNote((WindowsKeyCodes)keyCode))
			continue;

		// Pressed
		bool isPressed = GetAsyncKeyState(keyCode) & 0x8000;

		// Midi Note
		int midiNote = configuration->GetMidiNote((WindowsKeyCodes)keyCode);

		// Engage / Dis-Engage
		if (_synthDevice->HasNote(midiNote))
			_synthDevice->SetNote(midiNote, isPressed, streamTime);
	}
}
bool AudioController::Dispose()
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	try
	{
		delete _synthDevice;
		delete _streamClock;
		delete _audioTimer;

		_synthDevice = NULL;
		_streamClock = NULL;
		_audioTimer = NULL;
		_synthIntervalTimer = NULL;

		_initialized = false;

		return true;
	}
	catch (std::exception ex)
	{
		return false;
	}

	return false;
}

void AudioController::GetUpdate(float& streamTime, float& audioTime, float& frontendTime, float& latency, float& left, float& right)
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	streamTime = _streamClock->GetTime();
	audioTime = _audioTimer->GetAvgMilli();
	frontendTime = _synthIntervalTimer->AvgMilli();
	latency = 0;
	left = _outputL;
	right = _outputR;
}
