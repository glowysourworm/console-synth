#include "CompressorUI.h"
#include "Constant.h"
#include "DelayUI.h"
#include "Envelope.h"
#include "EnvelopeFilterUI.h"
#include "EnvelopeUI.h"
#include "LoopTimer.h"
#include "OscillatorUI.h"
#include "PlaybackBuffer.h"
#include "PlaybackClock.h"
#include "PlaybackParameters.h"
#include "ReverbUI.h"
#include "RtAudio.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"
#include "WindowsKeyCodes.h"
#include <Windows.h>
#include <exception>
#include <format>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

RtAudio* _rtAudio;
RtAudio::DeviceInfo _rtOutputDevice;
PlaybackParameters* _outputParameters;
PlaybackBuffer<float>* _outputStream;
SynthPlaybackDevice<float>* _synthDevice;
SynthConfiguration* _configuration;
PlaybackClock* _streamClock;
LoopTimer* _uiTimer;
LoopTimer* _audioTimer;


void ProcessKeyStrokes(double streamTime)
{
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

		if (!_configuration->HasMidiNote((WindowsKeyCodes)keyCode))
			continue;

		// Pressed
		bool isPressed = GetAsyncKeyState(keyCode) & 0x8000;

		// Midi Note
		int midiNote = _configuration->GetMidiNote((WindowsKeyCodes)keyCode);

		// Dis-Engage
		if (_synthDevice->HasNote(midiNote) && !isPressed)
			_synthDevice->SetNote(midiNote, false, streamTime);

		// Engage
		else if (!_synthDevice->HasNote(midiNote) && isPressed)
			_synthDevice->SetNote(midiNote, true, streamTime);
	}

	// Clean Up Synth Notes
	_synthDevice->ClearUnused(streamTime);
}

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
int PrimaryAudioCallback(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData)
{
	// Windows API, SynthConfiguration*, SynthPlaybackDevice* (be aware of usage)
	//
	ProcessKeyStrokes(streamTime);

	return _synthDevice->WritePlaybackBuffer(outputBuffer, nFrames, streamTime);
}


void PrimaryErrorCallback(RtAudioErrorType type, const std::string& errorText)
{

}

bool InitializeAudioClient()
{
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

		_rtAudio = new RtAudio(RtAudio::Api::WINDOWS_WASAPI, &PrimaryErrorCallback);

		RtAudio::StreamParameters outputParameters;

		// Output Device
		//
		auto outputDeviceIndex = _rtAudio->getDefaultOutputDevice();
		auto outputDevice = _rtAudio->getDeviceInfo(outputDeviceIndex);

		outputParameters.deviceId = outputDevice.ID;
		outputParameters.nChannels = outputDevice.outputChannels;

		// Output Buffer Calculation: ~device period (ms) * (s / ms) * (samples / s) = [samples]
		//
		// RT Audio:  (see openStream comments) will try to calculate a desired buffer size based on this input
		//			  value. So, we'll send it something the device likes; and see what it comes back with.
		//
		unsigned int outputBufferFrameSize = (unsigned int)(10.6667 * 0.001 * outputDevice.preferredSampleRate);
		unsigned int frontendFrameSize = outputBufferFrameSize;

		_rtAudio->openStream(&outputParameters,		// 
			NULL,									// Duplex Mode (input parameters)
			RTAUDIO_FLOAT32,						// RT Audio Format
			outputDevice.preferredSampleRate,		// Device Sampling Rate
			&outputBufferFrameSize,					// Device (preferred) Frame Size (RT Audio will adjust this)
			&PrimaryAudioCallback,					// Audio Callback
			_configuration);						// SynthConfiguration* Shared Pointer***

		// Initialize Playback Stream
		_outputStream = new PlaybackBuffer<float>(2,
			SAMPLING_RATE,
			AUDIO_BUFFER_SIZE * 100000,
			8,
			SIGNAL_LOW,
			SIGNAL_HIGH);

		_rtOutputDevice = outputDevice;

		// Store output parameters
		_outputParameters = new PlaybackParameters();
		//_outputParameters->hostAPI = new std::string("RT Audio (WASAPI)");
		_outputParameters->numberOfChannels = _rtOutputDevice.outputChannels;
		_outputParameters->samplingRate = _rtOutputDevice.preferredSampleRate;
		_outputParameters->outputBufferFrameSize = outputBufferFrameSize;
	}
	catch (std::exception ex)
	{
		return false;
	}

	return true;
}

bool DisposeAudioClient()
{
	try
	{
		if (_rtAudio == NULL)
			return true;

		if (_rtAudio->isStreamRunning())
			_rtAudio->stopStream();

		if (_rtAudio->isStreamOpen())
			_rtAudio->closeStream();

		delete _rtAudio;
		delete _outputStream;
		delete _synthDevice;
		delete _configuration;
		delete _streamClock;
		delete _uiTimer;
		delete _audioTimer;

		_rtAudio = NULL;
		_outputStream = NULL;
		_synthDevice = NULL;
		_configuration = NULL;
		_streamClock = NULL;
		_uiTimer = NULL;
		_audioTimer = NULL;
	}
	catch (std::exception ex)
	{
		return false;
	}

	return true;
}

/// <summary>
/// Initialization function for the synth backend. This must be called before starting the player!
/// </summary>
void InitializePlayback()
{
	_configuration = new SynthConfiguration();
	_synthDevice = new SynthPlaybackDevice<float>();
	_streamClock = new PlaybackClock();
	_uiTimer = new LoopTimer(0.025);
	_audioTimer = new LoopTimer(_outputParameters->outputBufferFrameSize / (double)_outputParameters->samplingRate);

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

	_synthDevice->Initialize(*_outputParameters);
	_rtAudio->startStream();
}

void LoopUI()
{
	// Reset All Timers
	_streamClock->Start();
	_streamClock->Mark();
	_uiTimer->Reset();
	_audioTimer->Reset();

	// Port Audio Variables
	//std::string version = "Unknown";
	std::string hostApi = "RT Audio (WASAPI)";
	std::string hostApiFormat = "Float (32 bit / channel)";
	double streamLatency = 0;

	// FTX-UI (Terminal Loop / Renderer)
	// 
	// Their backend will handle interaction, resizing, and redrawing. The document should be
	// updated on this thread (AFAIK) which is captured in the lambda function.
	//
	// https://arthursonzogni.github.io/FTXUI/
	//

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	// Source Oscillator
	OscillatorUI oscillatorUI("Oscillator", true, ftxui::Color::Blue);

	// Note Envelope
	EnvelopeUI envelopeUI(_configuration->GetNoteEnvelope(), "Input Envelope", true, ftxui::Color::BlueLight);

	// Envelope Filter (Oscillator)
	EnvelopeFilterUI envelopeFilterUI(_configuration->GetEnvelopeFilterType(),
									  _configuration->GetEnvelopeFilterOscillatorType(), 
									  _configuration->GetEnvelopeFilter(), 
									  _configuration->GetEnvelopeFilterCutoff(), 
									  _configuration->GetEnvelopeFilterResonance(), 
									  _configuration->GetEnvelopeFilterOscillatorFrequency(), 
									  _configuration->GetHasEnvelopeFilter(), 
									  "Envelope Filter", ftxui::Color::Red);
	// Reverb
	ReverbUI reverbUI(_configuration->GetHasReverb(),
					  _configuration->GetReverbDelaySeconds(), 
					  _configuration->GetReverbGain(), 
					  "Reverb", ftxui::Color::Green);

	// Delay
	DelayUI delayUI(_configuration->GetHasDelay(),
					_configuration->GetDelayFeedback(),
					_configuration->GetDelaySeconds(),
					_configuration->GetDelayGain(),
					"Delay", ftxui::Color::Purple);

	// Compressor
	CompressorUI compressorUI(_configuration->GetHasCompressor(),
							  _configuration->GetCompressorThreshold(), 
							  _configuration->GetCompressorGain(), 
							  _configuration->GetCompressorAttack(), 
							  _configuration->GetCompressorRelease(), 
							  _configuration->GetCompressionRatio(), 
							  "Compressor", ftxui::Color::HotPink);

	auto synthInputSettings = ftxui::Container::Horizontal({

		oscillatorUI.GetRenderer() | ftxui::flex_grow | ftxui::border,
		//envelopeUI.GetRenderer() | ftxui::flex_grow | ftxui::border,
		//envelopeFilterUI.GetRenderer() | ftxui::flex_grow | ftxui::border

	}) | ftxui::flex_grow;

	auto synthOutputSettings = ftxui::Container::Horizontal({

		compressorUI.GetRenderer() | ftxui::flex_grow | ftxui::border,
		reverbUI.GetRenderer() | ftxui::flex_grow | ftxui::border,
		delayUI.GetRenderer() | ftxui::flex_grow | ftxui::border

	});

	auto synthSettings = ftxui::Container::Vertical({
		synthInputSettings,
		//synthOutputSettings
	});

	// UI BACKEND LOOP!! This will be run just for re-drawing purposes during our
	//					 primary loop below.
	//
	auto renderer = ftxui::Renderer(synthSettings, [&]
	{
		// Synth Information
		auto synthInformation = ftxui::vbox(
		{
			ftxui::text("Host API:				   " + hostApi),
			ftxui::text("Host API Format:          " + hostApiFormat),
			ftxui::text("Current Time     (s):     " + std::format("{:.3f}", _streamClock->GetTime())),
			ftxui::text("Avg. UI Time     (ms):    " + std::format("{:.3f}", _uiTimer->GetAvgMilli())),
			ftxui::text("Avg. Sample Time (ms):    " + std::format("{:.3f}", _audioTimer->GetAvgMilli())),
			//ftxui::text("Avg. Sample Frames   :    " + std::format("{:.3f}", averageSampleWrite)),
			ftxui::text("Stream Latency   (ms):    " + std::format("{:.3f}", streamLatency)),
			ftxui::text("Sample Rate      (Hz):    " + std::to_string(_outputParameters->samplingRate))

		}) | ftxui::border;


		// Borderless Layout Grid
		auto container = ftxui::vbox({

			ftxui::text("Terminal Synth") | ftxui::color(ftxui::Color(0,255,0,255)),

			synthInformation,
			synthSettings->Render()

		}) | ftxui::flex_grow;

		return container;
	});

	// Initialize Screen (sizing)
	screen.FitComponent();

	// FTXUI has an option to create an event loop (this will run their backend UI code)
	//
	// https://arthursonzogni.com/FTXUI/doc/examples_2component_2custom_loop_8cpp-example.html#_a8
	//
	ftxui::Loop loop(&screen, renderer);

	// Primary Loop!!! We'll handle this loop - using our system timer to manage the 
	//				   accuracy of the audio output; and also throttle events for the
	//				   UI's backend. We should be redrawing ~10ms
	while (!loop.HasQuitted())
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			screen.ExitLoopClosure();
			break;
		}

		// Synth Configuration:  Our copy is updated on the UI Update timer
		//
		if (_uiTimer->Mark())
		{
			// Force timer to wait until next goal is reached
			//
			_uiTimer->Reset();

			// Use custom event to force one UI update
			screen.PostEvent(ftxui::Event::Custom);

			// Update Configuration
			//_configuration->SetNoteEnvelope(Envelope(noteAttack, noteDecay, noteSustain, noteRelease, envelope.GetAttackPeak(), envelope.GetSustainPeak()));
			//_configuration->SetEnvelopeFilter(Envelope(filterAttack, filterDecay, filterSustain, filterRelease, filter.GetAttackPeak(), filter.GetSustainPeak()));
			//_configuration->SetOscillatorType((AmplitudeOscillatorType)oscillatorChoice);
			//_configuration->SetEnvelopeFilterType((EnvelopeFilterType)envelopeFilterTypeChoice);
			//_configuration->SetEnvelopeFilterCutoff(filterCutoff);
			//_configuration->SetEnvelopeFilterResonance(filterResonance);
			//_configuration->SetEnvelopeFilterOscillatorFrequency(filterOscillatorFrequency);
			//_configuration->SetEnvelopeFilterOscillatorType((AmplitudeOscillatorType)envelopeOscillatorChoice);
			//_configuration->SetHasEnvelopeFilter(envelopeFilterEnabled == 0);
			//_configuration->SetHasDelay(delayEnabled == 0);
			//_configuration->SetHasCompressor(compressorEnabled == 0);
			//_configuration->SetHasReverb(reverbEnabled == 0);
			//_configuration->SetReverbSeconds(reverbDelaySeconds);
			//_configuration->SetReverbGain(reverbGain);
			//_configuration->SetDelayFeedback(delayFeedbackEnabled == 0);
			//_configuration->SetDelaySeconds(delaySeconds);
			//_configuration->SetDelayGain(delayGain);
			//_configuration->SetCompressionRatio(compressorRatio);
			//_configuration->SetCompressorAttack(compressorAttack);
			//_configuration->SetCompressorGain(compressorGain);
			//_configuration->SetCompressorRelease(compressorRelease);
			//_configuration->SetCompressorThreshold(compressorThreshold);

			// UI Run
			loop.RunOnce();
		}

		// Only update if changes were made (~100ms)
		if (_configuration->IsDirty())
		{
			// CRITICAL SECTION:  This is an update from the UI, which will reset the synth parameters. So,
			//					  it is only allowed every ~100ms at the most.
			//

			//_player->StopStream();

			_synthDevice->UpdateSynth(*_configuration);

			//_player->StartStream();

			// Reset Configuration Flag
			_configuration->ClearDirty();
		}
		
		// THIS MOVES TO THE OTHER THREAD
		_audioTimer->Mark();

		//screen.ExitLoopClosure();
	}
}


int main(int argc, char* argv[], char* envp[])
{
	SetConsoleTitleA("Terminal Synth");

	// Read midi file
	if (argc > 1)
	{
	}

	// Manual keyboard input
	else
	{
		if (!InitializeAudioClient())
			return -1;

		InitializePlayback();
	}

	LoopUI();

	if (!DisposeAudioClient())
		return -1;

	return 0;
}