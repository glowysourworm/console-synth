#include "AudioController.h"
#include "IntervalTimer.h"
#include "LoopTimer.h"
#include "MidiPlaybackDevice.h"
#include "PlaybackClock.h"
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"
#include <exception>
#include <string>

AudioController::AudioController()
{
	_initialized = false;
	_midiMode = false;

	_synthDevice = new SynthPlaybackDevice<float>();
	_midiDevice = new MidiPlaybackDevice<float>();
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
	_midiDevice->Initialize(configuration, parameters);

	_streamClock->Reset();
	_streamClock->Start();
	_audioTimer->Reset();

	_initialized = true;

	// Only block callback during mode switching
	_blockCallback = false;

	return true;
}

int AudioController::ProcessAudioCallback(float* outputBuffer, unsigned int numberOfFrames, double streamTime, SynthConfiguration* configuration)
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	// Full Audio Loop Timer
	_audioTimer->Mark();

	// Mode Switching (MIDI / Synth)
	if (_blockCallback)
		return 0;

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

	int rtAudioReturnValue = 0;

	// Last Output
	bool lastOutput = _midiMode ? _midiDevice->GetLastOutput() : _synthDevice->GetLastOutput();

	// Windows API, SynthConfiguration*, SynthPlaybackDevice* (be aware of usage)
	//
	bool pressedKeys = _midiMode ? _midiDevice->SetForPlayback(numberOfFrames, streamTime, configuration) :
								   _synthDevice->SetForPlayback(numberOfFrames, streamTime, configuration);

	// Optimize CPU
	if (lastOutput || pressedKeys)
	{
		// Write playback buffer from synth device
		rtAudioReturnValue = _midiMode ? _midiDevice->WritePlaybackBuffer(outputBuffer, numberOfFrames, streamTime, configuration) :
										 _synthDevice->WritePlaybackBuffer(outputBuffer, numberOfFrames, streamTime, configuration);

		// Get output for the UI
		_outputL = _synthDevice->GetOutput(0);
		_outputR = _synthDevice->GetOutput(1);
	}
	else
	{
		_outputL = 0;
		_outputR = 0;
	}

	// std::atomic end loop (this should only run once!)
	while (!configuration->SetWait(false)) {}

	// Frontend Processing Time (Mark.)
	_synthIntervalTimer->Mark();

	return rtAudioReturnValue;
}

bool AudioController::Dispose()
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	try
	{
		delete _synthDevice;
		delete _midiDevice;
		delete _streamClock;
		delete _audioTimer;

		_midiDevice = NULL;
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

void AudioController::SetMidiMode(const std::string& midiFile)
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	// std::atomic block to prepare midi playback device
	_blockCallback = true;

	_midiMode = true;

	// Prepare MIDI Playback
	_midiDevice->Load(midiFile);

	_blockCallback = false;
}

void AudioController::SetSynthMode()
{
	if (!_initialized)
		throw new std::exception("Audio Controller not yet initialized!");

	// std::atomic block to prepare synth playback device
	_blockCallback = true;

	_midiMode = false;

	// Nothing else to do..

	_blockCallback = false;
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
