#include "Constant.h"
#include "Envelope.h"
#include "IntervalTimer.h"
#include "LoopTimer.h"
#include "PlaybackBuffer.h"
#include "PlaybackClock.h"
#include "PlaybackParameters.h"
#include "RtAudio.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"
#include "WindowsKeyCodes.h"
#include <Windows.h>
#include <chrono>
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
#include <thread>
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
IntervalTimer* _synthIntervalTimer;

float _frontendAvgMilli;

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
	// Full Audio Loop Timer
	_audioTimer->Mark();

	// Frontend Processing Time (Start!)
	_synthIntervalTimer->Reset();

	// Windows API, SynthConfiguration*, SynthPlaybackDevice* (be aware of usage)
	//
	ProcessKeyStrokes(streamTime);

	int returnValue = _synthDevice->WritePlaybackBuffer(outputBuffer, nFrames, streamTime);

	// Frontend Processing Time (Mark.)
	_synthIntervalTimer->Mark();

	_frontendAvgMilli = _synthIntervalTimer->AvgMilli();

	return returnValue;
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

		_rtAudio->openStream(& outputParameters,	// 
			NULL,									// Duplex Mode (input parameters)
			RTAUDIO_FLOAT32,						// RT Audio Format
			outputDevice.preferredSampleRate,		// Device Sampling Rate
			&outputBufferFrameSize,					// Device (preferred) Frame Size (RT Audio will adjust this)
			&PrimaryAudioCallback,					// Audio Callback
			_configuration,							// SynthConfiguration* Shared Pointer***
			&options);						

		// Initialize Playback Stream
		_outputStream = new PlaybackBuffer<float>(2,
			outputDevice.preferredSampleRate,
			outputBufferFrameSize,
			8,
			SIGNAL_LOW,
			SIGNAL_HIGH);

		_rtOutputDevice = outputDevice;

		//std::string deviceFormat("Float32 - Channels");

		const char* deviceFormat = "Bizzare Unknown";

		// Store output parameters
		_outputParameters = new PlaybackParameters(
			_rtAudio->getApiDisplayName(_rtAudio->getCurrentApi()).c_str(),
			deviceFormat,
			_rtOutputDevice.name.c_str(),
			_rtOutputDevice.preferredSampleRate, 
			_rtOutputDevice.outputChannels, 
			outputBufferFrameSize);
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
		_synthIntervalTimer = NULL;
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
	_audioTimer = new LoopTimer(_outputParameters->GetOutputBufferFrameSize() / (double)_outputParameters->GetSamplingRate());
	_synthIntervalTimer = new IntervalTimer();

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

	// FTX-UI (Terminal Loop / Renderer)
	// 
	// Their backend will handle interaction, resizing, and redrawing. The document should be
	// updated on this thread (AFAIK) which is captured in the lambda function(s). 
	// 
	// After more working with FTXUI, it is using std::move, and std::make_shared, and std::shared_ptr
	// to try to keep stack allocations, and move them to the heap. I'm not as familiar with this pattern;
	// but it is very difficult to reproduce an inheritance pattern. Though, not impossible, it would
	// be a better use of time to just leave it on our stack, which will accomplish the same task. 
	// 
	// There is a slight memory leak which may be in FTXUI; but I'm not sure, yet, if RT Audio has issues,
	// or just our SynthNote* allocations (probably not).
	//
	// https://arthursonzogni.github.io/FTXUI/
	//

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	double streamLatency = 0;

	// Synth Configuration
	bool configurationChange = false;
	Envelope envelope = _configuration->GetNoteEnvelope();
	Envelope filter = _configuration->GetEnvelopeFilter();

	float lineMin = 0.00f;
	float lineMax = 1.00f;
	float lineIncr = 0.01f;

	float envelopeMin = 0.01f;
	float envelopeMax = 3.0f;
	float envelopeIncrement = 0.01f;

	float filterCutoffMin = 60;
	float filterCutoffMax = 7000;
	float filterCutoffIncr = 10;

	float noteAttack = envelope.GetAttack();
	float noteSustain = envelope.GetSustain();
	float noteDecay = envelope.GetDecay();
	float noteRelease = envelope.GetRelease();

	float filterAttack = filter.GetAttack();
	float filterSustain = filter.GetSustain();
	float filterDecay = filter.GetDecay();
	float filterRelease = filter.GetRelease();

	float filterCutoff = _configuration->GetEnvelopeFilterCutoff();
	float filterResonance = _configuration->GetEnvelopeFilterResonance();

	float filterOscillatorFrequency = _configuration->GetEnvelopeFilterOscillatorFrequency();
	float filterOscillatorFrequencyLow = 0.5;
	float filterOscillatorFrequencyHigh = 5;
	float filterOscillatorFrequencyIncr = 0.1;

	float delaySeconds = _configuration->GetDelaySeconds();
	float delayGain = _configuration->GetDelayGain();
	float delaySecondsMin = 0.1f;
	float delaySecondsMax = 3.0f;
	float delaySecondsIncr = 0.1f;

	float reverbDelaySeconds = _configuration->GetReverbDelaySeconds();
	float reverbGain = _configuration->GetReverbGain();

	float reverbDelayMin = 0.001f;
	float reverbDelayMax = 0.500f;
	float reverbDelayIncr = 0.001f;

	float compressorGain = _configuration->GetCompressorGain();
	float compressorThreshold = _configuration->GetCompressorThreshold();
	float compressorAttack = _configuration->GetCompressorAttack();
	float compressorRelease = _configuration->GetCompressorRelease();
	float compressorRatio = _configuration->GetCompressionRatio();

	float compressorGainLow = 0.0f;
	float compressorGainHigh = 10.0f;
	float compressorGainIncr = 0.01f;

	float compressorRatioLow = 1.0f;
	float compressorRatioHigh = 10.0f;
	float compressorRatioIncr = 1.0f;

	std::string attackStr;
	std::string sustainStr;
	std::string decayStr;
	std::string releaseStr;

	std::string filterAttackStr;
	std::string filterSustainStr;
	std::string filterDecayStr;
	std::string filterReleaseStr;

	std::string filterCutoffStr;
	std::string filterResonanceStr;

	std::string filterOscillatorStr;

	std::string reverbDelayStr;
	std::string reverbGainStr;

	std::string delayStr;
	std::string delayGainStr;

	std::string compressorGainStr;
	std::string compressorThresholdStr;
	std::string compressorRatioStr;
	std::string compressorAttackStr;
	std::string compressorReleaseStr;

	int oscillatorChoice = (int)_configuration->GetOscillatorType();
	int envelopeFilterEnabled = _configuration->GetHasEnvelopeFilter() ? 0 : 1;
	int envelopeFilterTypeChoice = (int)_configuration->GetEnvelopeFilterType();
	int envelopeOscillatorChoice = (int)_configuration->GetEnvelopeFilterOscillatorType();

	int reverbEnabled = _configuration->GetHasReverb() ? 0 : 1;

	int delayEnabled = _configuration->GetHasDelay() ? 0 : 1;
	int delayFeedbackEnabled = _configuration->GetDelayFeedback() ? 0 : 1;

	int compressorEnabled = _configuration->GetHasCompressor() ? 0 : 1;

	auto oscillatorStrs = std::vector<std::string>(
	{
		"Sine",
		"Square",
		"Triangle",
		"Sawtooth",
		"Random"
	});
	auto onOffStrs = std::vector<std::string>(
	{
		"On",
		"Off"
	});
	auto envelopeFilterTypeStrs = std::vector<std::string>(
	{
		"Manual",
		"Oscillator",
		"Sweep"
	});

	// Oscillator
	auto oscillatorUI = ftxui::Radiobox(&oscillatorStrs, &oscillatorChoice);

	// Note Envelope
	auto envelopeUI = ftxui::Container::Vertical(
	{
		ftxui::Renderer([&] { return ftxui::text("Input Envelope") | ftxui::color(ftxui::Color(0,88,255,255)); }),
		ftxui::Renderer([&] { return ftxui::separator(); }),
		ftxui::Slider(&attackStr, &noteAttack, envelopeMin, envelopeMax, envelopeIncrement) | ftxui::color(ftxui::Color::White),
		ftxui::Slider(&sustainStr, &noteSustain, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider(&decayStr, &noteDecay, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider(&releaseStr, &noteRelease, envelopeMin, envelopeMax, envelopeIncrement),
	});

	// Envelope Filter Label (Enable)
	auto envelopeFilterLabelUI = ftxui::Container::Horizontal(
	{
		ftxui::Renderer([&] { return ftxui::text("Envelope Filter") | ftxui::color(ftxui::Color(255,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &envelopeFilterEnabled) | ftxui::align_right,
	});

	// Envelope Filter Type Label (Toggle)
	auto envelopeFilterTypeLabelUI = ftxui::Container::Horizontal(
	{
		ftxui::Renderer([&] { return ftxui::text("Filter Type") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(envelopeFilterTypeStrs, &envelopeFilterTypeChoice) | ftxui::align_right,
	});

	// Envelope Filter (Oscillator)
	auto envelopeOscillatorUI = ftxui::Container::Vertical(
	{
		ftxui::Renderer([&] { return ftxui::text("Oscillator Type (VCO)"); }),
		ftxui::Renderer([&] { return ftxui::separator(); }),
		ftxui::Radiobox(&oscillatorStrs, &envelopeOscillatorChoice),
		ftxui::Renderer([&] { return ftxui::separator(); }),
		ftxui::Slider(&filterOscillatorStr, &filterOscillatorFrequency, filterOscillatorFrequencyLow, filterOscillatorFrequencyHigh, filterOscillatorFrequencyIncr),
	});

	// Envelope Filter (Sweep)
	auto envelopeSweepUI = ftxui::Container::Vertical(
	{
		ftxui::Slider(&filterAttackStr, &filterAttack, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider(&filterSustainStr, &filterSustain, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider(&filterDecayStr, &filterDecay, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider(&filterReleaseStr, &filterRelease, envelopeMin, envelopeMax, envelopeIncrement),
	});

	// Reverb (Enable)
	auto reverbEnableUI = ftxui::Container::Horizontal(
	{
		ftxui::Renderer([&] { return ftxui::text("Reverb") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &reverbEnabled) | ftxui::align_right,
	});

	// Reverb
	auto reverbUI = ftxui::Container::Vertical(
	{
		reverbEnableUI,
		ftxui::Renderer([&] { return ftxui::separator(); }),

		// Delay (s)
		ftxui::Slider(&reverbDelayStr, &reverbDelaySeconds, reverbDelayMin, reverbDelayMax, reverbDelayIncr),

		// Gain
		ftxui::Slider(&reverbGainStr, &reverbGain, lineMin, lineMax, lineIncr),
	});

	// Delay (Enable)
	auto delayEnableUI = ftxui::Container::Horizontal(
	{
		ftxui::Renderer([&] { return ftxui::text("Delay") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &delayEnabled) | ftxui::align_right,
	});

	// Delay Feedback Enable (Toggle)
	auto delayFeedbackUI = ftxui::Container::Horizontal(
	{
		ftxui::Renderer([&] { return ftxui::text("Feedback") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &delayFeedbackEnabled) | ftxui::align_right,
	});

	// Delay
	auto delayUI = ftxui::Container::Vertical(
	{
		delayEnableUI,
		ftxui::Renderer([&] { return ftxui::separator(); }),

		// Feedback (Enable)
		delayFeedbackUI,
		ftxui::Renderer([&] { return ftxui::separator(); }),

		// Delay (s)
		ftxui::Slider(&delayStr, &delaySeconds, delaySecondsMin, delaySecondsMax, delaySecondsIncr),

		// Gain
		ftxui::Slider(&delayGainStr, &delayGain, lineMin, lineMax, lineIncr),
	});

	// Compressor (Enable)
	auto compressorEnableUI = ftxui::Container::Horizontal(
	{
		ftxui::Renderer([&] { return ftxui::text("Compressor") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &compressorEnabled) | ftxui::align_right,
	});

	// Compressor
	auto compressorUI = ftxui::Container::Vertical(
	{
		compressorEnableUI,
		ftxui::Renderer([&] { return ftxui::separator(); }),

		// Threshold
		ftxui::Slider(&compressorThresholdStr, &compressorThreshold, lineMin, lineMax, lineIncr),

		// Gain
		ftxui::Slider(&compressorGainStr, &compressorGain, compressorGainLow, compressorGainHigh, compressorGainIncr),

		// Attack
		ftxui::Slider(&compressorAttackStr, &compressorAttack, lineMin, lineMax, lineIncr),

		// Release
		ftxui::Slider(&compressorReleaseStr, &compressorRelease, lineMin, lineMax, lineIncr),

		// Compression Ratio
		ftxui::Slider(&compressorRatioStr, &compressorRatio, compressorRatioLow, compressorRatioHigh, compressorRatioIncr),
	});

	// Envelope Type Chooser
	auto envelopeTypeUI = ftxui::Container::Vertical(
	{
		// Manual 
		ftxui::Renderer([&] { return ftxui::text("Manual Settings"); }) | ftxui::Maybe([&] { return envelopeFilterTypeChoice == 0; }),

		// Oscillator
		envelopeOscillatorUI | ftxui::Maybe([&] { return envelopeFilterTypeChoice == 1; }),

		// Sweep
		envelopeSweepUI | ftxui::Maybe([&] { return envelopeFilterTypeChoice == 2; })
	});

	// Envelope Filter
	auto envelopeFilterUI = ftxui::Container::Vertical(
	{
		envelopeFilterLabelUI,
		ftxui::Renderer([&] {return ftxui::separator(); }),

		envelopeFilterTypeLabelUI,
		ftxui::Renderer([&] {return ftxui::separator(); }),

		envelopeTypeUI,

		ftxui::Renderer([&] {return ftxui::separator(); }),

		ftxui::Slider(&filterCutoffStr, &filterCutoff, filterCutoffMin, filterCutoffMax, filterCutoffIncr),
		ftxui::Slider(&filterResonanceStr, &filterResonance, lineMin, lineMax, lineIncr),
	});

	// Source Oscillator
	auto oscillatorUIRenderer = ftxui::Renderer(oscillatorUI, [&]
	{
		return ftxui::vbox(
		{
			ftxui::text("Oscillator") | ftxui::color(ftxui::Color(0, 88, 255, 255)),
			ftxui::separator(),
			oscillatorUI->Render()
		});
	});

	// Source Envelope
	auto envelopeUIRenderer = ftxui::Renderer(envelopeUI, [&]
	{
		attackStr = "Attack  (s) " + std::format("{:.2f}", noteAttack);
		sustainStr = "Sustain (s) " + std::format("{:.2f}", noteSustain);
		decayStr = "Decay   (s) " + std::format("{:.2f}", noteDecay);
		releaseStr = "Release (s) " + std::format("{:.2f}", noteRelease);

		return envelopeUI->Render();
	});

	// Envelope Filter
	auto filterUIRenderer = ftxui::Renderer(envelopeFilterUI, [&]
	{
		filterAttackStr = "Attack  (s) " + std::format("{:.2f}", filterAttack);
		filterSustainStr = "Sustain (s) " + std::format("{:.2f}", filterSustain);
		filterDecayStr = "Decay   (s) " + std::format("{:.2f}", filterDecay);
		filterReleaseStr = "Release (s) " + std::format("{:.2f}", filterRelease);

		filterCutoffStr = "Cutoff (Hz) " + std::to_string((int)filterCutoff);
		filterResonanceStr = "Resonance   " + std::format("{:.2f}", filterResonance);

		filterOscillatorStr = "Oscillator (Hz) " + std::format("{:.1f}", filterOscillatorFrequency);

		return envelopeFilterUI->Render();
	});

	// Reverb
	auto reverbUIRenderer = ftxui::Renderer(reverbUI, [&] 
	{
		reverbDelayStr = "Delay (s) " + std::format("{:.4f}", reverbDelaySeconds);
		reverbGainStr = "Gain      " + std::format("{:.2f}", reverbGain) + "  ";

		return reverbUI->Render();
	});

	// Delay
	auto delayUIRenderer = ftxui::Renderer(delayUI, [&] 
	{
		delayStr = "Delay (s) " + std::format("{:.2f}", delaySeconds);
		delayGainStr = "Gain      " + std::format("{:.2f}", delayGain);

		return delayUI->Render();
	});

	// Compressor
	auto compressorUIRenderer = ftxui::Renderer(compressorUI, [&] 
	{
		compressorThresholdStr = "Threshold    " + std::format("{:.2f}", compressorThreshold) + " ";
		compressorGainStr = "Gain         " + std::format("{:.2f}", compressorGain) + " ";
		compressorAttackStr = "Attack  (s)  " + std::format("{:.2f}", compressorAttack) + " ";
		compressorReleaseStr = "Release (s)  " + std::format("{:.2f}", compressorRelease) + " ";

		// Align Labels
		if (compressorRatio == 10.0f)
			compressorRatioStr = "Comp. Ratio  " + std::format("{:.2f}", compressorRatio);

		else
			compressorRatioStr = "Comp. Ratio  " + std::format("{:.2f}", compressorRatio) + " ";

		return compressorUI->Render();
	});

	auto synthInputSettings = ftxui::Container::Horizontal({

		oscillatorUIRenderer | ftxui::flex_grow | ftxui::border,
		envelopeUIRenderer | ftxui::flex_grow | ftxui::border,
		filterUIRenderer | ftxui::flex_grow | ftxui::border

	}) | ftxui::flex_grow;

	auto synthOutputSettings = ftxui::Container::Horizontal({

		compressorUIRenderer | ftxui::flex_grow | ftxui::border,
		reverbUIRenderer | ftxui::flex_grow | ftxui::border,
		delayUIRenderer | ftxui::flex_grow | ftxui::border

	});

	auto synthSettings = ftxui::Container::Vertical({
		synthInputSettings,
		synthOutputSettings
	});

	// UI BACKEND LOOP!! This will be run just for re-drawing purposes during our
	//					 primary loop below.
	//
	auto renderer = ftxui::Renderer(synthSettings, [&]
	{
		// Terminal Synth Information
		auto synthInformation = ftxui::hbox(
		{
			ftxui::vbox(
			{
				ftxui::text("Host API:                 " + _outputParameters->GetHostApi()),
				ftxui::text("Device Name:              " + _outputParameters->GetDeviceName()),
				ftxui::text("Stream Format:            " + _outputParameters->GetDeviceFormat()),
				ftxui::text("Stream Buffer Size:       " + std::format("{} (frames)", _outputParameters->GetOutputBufferFrameSize())),
				ftxui::text("Sample Rate (Hz):         " + std::to_string(_outputParameters->GetSamplingRate()))

			}) | ftxui::flex_grow,

			ftxui::separator(),

			ftxui::vbox(
			{
				ftxui::text(" Stream Time            (s):    " + std::format("{:.3f}", _streamClock->GetTime())),
				ftxui::text(" Avg. UI Time          (ms):    " + std::format("{:.3f}", _uiTimer->GetAvgMilli())),
				ftxui::text(" Avg. Callback Time    (ms):    " + std::format("{:.3f}", _audioTimer->GetAvgMilli())),
				ftxui::text(" Avg. Frontend Time    (ms):    " + std::format("{:.3f}", _synthIntervalTimer->AvgMilli())),
				ftxui::text(" Stream Latency   (samples):    " + std::to_string(_rtAudio->getStreamLatency()))

			}) | ftxui::flex_grow
		});


		// Borderless Layout Grid
		auto container = ftxui::vbox({

			ftxui::vbox({
				ftxui::text("Terminal Synth") | ftxui::color(ftxui::Color(0,255,0,255)),
				ftxui::separator(),
				synthInformation,

			}) | ftxui::border | ftxui::flex_grow,

			synthSettings->Render() | ftxui::border | ftxui::flex_grow

		});

		return container;
	});


	// Initialize Screen (sizing)
	screen.FitComponent();

	// FTXUI has an option to create an event loop (this will run their backend UI code)
	//
	// https://arthursonzogni.com/FTXUI/doc/examples_2component_2custom_loop_8cpp-example.html#_a8
	//
	ftxui::Loop loop(& screen, renderer);

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
			_configuration->SetNoteEnvelope(Envelope(noteAttack, noteDecay, noteSustain, noteRelease, envelope.GetAttackPeak(), envelope.GetSustainPeak()));
			_configuration->SetEnvelopeFilter(Envelope(filterAttack, filterDecay, filterSustain, filterRelease, filter.GetAttackPeak(), filter.GetSustainPeak()));
			_configuration->SetOscillatorType((OscillatorType)oscillatorChoice);
			_configuration->SetEnvelopeFilterType((EnvelopeFilterType)envelopeFilterTypeChoice);
			_configuration->SetEnvelopeFilterCutoff(filterCutoff);
			_configuration->SetEnvelopeFilterResonance(filterResonance);
			_configuration->SetEnvelopeFilterOscillatorFrequency(filterOscillatorFrequency);
			_configuration->SetEnvelopeFilterOscillatorType((OscillatorType)envelopeOscillatorChoice);
			_configuration->SetHasEnvelopeFilter(envelopeFilterEnabled == 0);
			_configuration->SetHasDelay(delayEnabled == 0);
			_configuration->SetHasCompressor(compressorEnabled == 0);
			_configuration->SetHasReverb(reverbEnabled == 0);
			_configuration->SetReverbSeconds(reverbDelaySeconds);
			_configuration->SetReverbGain(reverbGain);
			_configuration->SetDelayFeedback(delayFeedbackEnabled == 0);
			_configuration->SetDelaySeconds(delaySeconds);
			_configuration->SetDelayGain(delayGain);
			_configuration->SetCompressionRatio(compressorRatio);
			_configuration->SetCompressorAttack(compressorAttack);
			_configuration->SetCompressorGain(compressorGain);
			_configuration->SetCompressorRelease(compressorRelease);
			_configuration->SetCompressorThreshold(compressorThreshold);

			// UI Run
			loop.RunOnce();
		}

		// Only update if changes were made (~100ms)
		if (_configuration->IsDirty())
		{
			// CRITICAL SECTION:  This is an update from the UI, which will reset the synth parameters. So,
			//					  it is only allowed every ~100ms at the most.
			//

			_rtAudio->stopStream();

			_synthDevice->UpdateSynth(*_configuration);

			// This timer is not thread-safe; but the usage seems to be ok on both threads
			// without stopping the audio stream. We need to re-measure the metrics up-to-date.
			_synthIntervalTimer->ClearAll();
			_streamClock->Reset();
			_uiTimer->Reset();
			_audioTimer->Reset();

			_rtAudio->startStream();

			// Reset Configuration Flag
			_configuration->ClearDirty();
		}
		
		std::this_thread::sleep_for(std::chrono::microseconds(100));
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