//#include "Accumulator.h"
//#include "CompressorUI.h"
//#include "Constant.h"
//#include "DelayUI.h"
//#include "Envelope.h"
//#include "EnvelopeFilterUI.h"
//#include "EnvelopeUI.h"
//#include "IntervalTimer.h"
//#include "LoopTimer.h"
//#include "OscillatorUI.h"
//#include "OutputUI.h"
//#include "PlaybackBuffer.h"
//#include "PlaybackClock.h"
//#include "PlaybackParameters.h"
//#include "ReverbUI.h"
//#include "RtAudio.h"
//#include "SynthConfiguration.h"
//#include "SynthPlaybackDevice.h"
//#include "WindowsKeyCodes.h"
//#include <Windows.h>
//#include <chrono>
//#include <exception>
//#include <format>
//#include <ftxui/component/component.hpp>
//#include <ftxui/component/component_base.hpp>
//#include <ftxui/component/event.hpp>
//#include <ftxui/component/loop.hpp>
//#include <ftxui/component/mouse.hpp>
//#include <ftxui/component/mouse.hpp>
//#include <ftxui/component/screen_interactive.hpp>
//#include <ftxui/dom/elements.hpp>
//#include <ftxui/screen/color.hpp>
//#include <string>
//#include <thread>
//
//RtAudio* _rtAudio;
//RtAudio::DeviceInfo _rtOutputDevice;
//PlaybackParameters* _outputParameters;
//PlaybackBuffer<float>* _outputStream;
//SynthPlaybackDevice<float>* _synthDevice;
//SynthConfiguration* _configuration;
//PlaybackClock* _streamClock;
//LoopTimer* _uiTimer;
//LoopTimer* _audioTimer;
//IntervalTimer* _synthIntervalTimer;
//
//// Related to last callback
//float _outputL;
//float _outputR;
//bool _hasOutput;
//
//
//void ProcessKeyStrokes(double streamTime)
//{
//	// Iterate Key Codes (probably the most direct method)
//	//
//	for (int keyCode = (int)WindowsKeyCodes::NUMBER_0; keyCode <= (int)WindowsKeyCodes::PERIOD; keyCode++)
//	{
//		// Check that enum is defined
//		if (keyCode < 0x30 ||
//			keyCode == 0x40 ||
//			(keyCode > 0x5A && keyCode < 0x80) ||
//			(keyCode > 0x80 && keyCode < 0xBB) ||
//			(keyCode > 0xBF && keyCode < 0xDB) ||
//			(keyCode > 0xDE))
//			continue;
//
//		if (!_configuration->HasMidiNote((WindowsKeyCodes)keyCode))
//			continue;
//
//		// Pressed
//		bool isPressed = GetAsyncKeyState(keyCode) & 0x8000;
//
//		// Midi Note
//		int midiNote = _configuration->GetMidiNote((WindowsKeyCodes)keyCode);
//
//		// Engage / Dis-Engage
//		if (_synthDevice->HasNote(midiNote))
//			_synthDevice->SetNote(midiNote, isPressed, streamTime);
//	}
//}
//
///// <summary>
///// Primary RT Audio Callback:  They have a separate thread managing the device audio. So, this will be on their thread; and we 
///// will process all of our SynthPlaybackDevice* work here - including key strokes. 
///// </summary>
///// <param name="outputBuffer">Output Audio Buffer (see RTAudio initialization)</param>
///// <param name="inputBuffer">Input Audio (used for recording or duplex mode)</param>
///// <param name="nFrames">Number of audio frames (Frame = { channel 1, .. channel N }, usually L/R channels)</param>
///// <param name="streamTime">Stream time in seconds</param>
///// <param name="status">RT Audio stream status</param>
///// <param name="userData">This will contain a pointer to the synth configuration. However, it is not thread safe!</param>
///// <returns>Error indicator to RT Audio</returns>
//int PrimaryAudioCallback(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData)
//{
//	// Full Audio Loop Timer
//	_audioTimer->Mark();
//
//	// Frontend Processing Time (Start!)
//	_synthIntervalTimer->Reset();
//
//	// Windows API, SynthConfiguration*, SynthPlaybackDevice* (be aware of usage)
//	//
//	ProcessKeyStrokes(streamTime);
//
//	int returnValue = _synthDevice->WritePlaybackBuffer(outputBuffer, nFrames, streamTime);
//
//	// Get output for the UI
//	if (_hasOutput)
//	{
//		_outputL = _synthDevice->GetOutput(0);
//		_outputR = _synthDevice->GetOutput(1);
//	}
//	else
//	{
//		_outputL = 0;
//		_outputR = 0;
//	}
//
//	// Frontend Processing Time (Mark.)
//	_synthIntervalTimer->Mark();
//
//	return returnValue;
//}
//
//
//void PrimaryErrorCallback(RtAudioErrorType type, const std::string& errorText)
//{
//
//}
//
//bool InitializeAudioClient()
//{
//	try
//	{
//		// Buffer Frame Calculation:  How many buffer frames will be appropriate?
//		//
//		// After some tedious work with the Windows WASAPI, and using our real time
//		// loop, it was obvious that there is a difficult problem trying to synchronize
//		// the audio backend with the frontend looping. 
//		//
//		// The approximate time for our primary loop is ~7ms. Rendering the actual 
//		// audio is a very small portion of this time, for synchronous setup.
//		//
//		// RT Audio callbacks are on a separate thread. So, we're going to try to
//		// manage our loop so the time to render audio is small. Also, there will
//		// be a shared pointer to the SynthConfiguration*, which will be carefully
//		// set by our thread, and read by the other. This is NOT a thread-safe 
//		// operation; but we won't care as long as we are just setting primitive 
//		// variables.
//		//
//		// The size of the RT Audio buffer should be set from the device. The WASAPI
//		// returned approx ~10.6667ms as the default period. So, we're going to select
//		// an audio buffer time of this much; and see how that works. Later, we'll 
//		// try to take this number from the device itself.
//		//
//
//		_rtAudio = new RtAudio(RtAudio::Api::WINDOWS_WASAPI, &PrimaryErrorCallback);
//
//		RtAudio::StreamParameters outputParameters;
//
//		// Output Device
//		//
//		auto outputDeviceIndex = _rtAudio->getDefaultOutputDevice();
//		auto outputDevice = _rtAudio->getDeviceInfo(outputDeviceIndex);
//
//		outputParameters.deviceId = outputDevice.ID;
//		outputParameters.nChannels = outputDevice.outputChannels;
//		outputParameters.firstChannel = 0;
//
//		RtAudio::StreamOptions options;
//
//		//options.flags |= RTAUDIO_SCHEDULE_REALTIME;					
//		options.numberOfBuffers = 4;						// Has to do with audio format!
//		options.flags |= RTAUDIO_HOG_DEVICE;
//		options.flags |= RTAUDIO_MINIMIZE_LATENCY;
//		// options.flags |= RTAUDIO_NONINTERLEAVED; 
//
//
//		// Output Buffer Calculation: ~device period (ms) * (s / ms) * (samples / s) = [samples]
//		//
//		// RT Audio:  (see openStream comments) will try to calculate a desired buffer size based on this input
//		//			  value. So, we'll send it something the device likes; and see what it comes back with.
//		//
//		//unsigned int outputBufferFrameSize = (unsigned int)(10.6667 * 0.001 * outputDevice.preferredSampleRate);
//		unsigned int outputBufferFrameSize = 16;
//		unsigned int frontendFrameSize = outputBufferFrameSize;
//
//		_rtAudio->openStream(& outputParameters,	// 
//			NULL,									// Duplex Mode (input parameters)
//			RTAUDIO_FLOAT32,						// RT Audio Format
//			outputDevice.preferredSampleRate,		// Device Sampling Rate
//			&outputBufferFrameSize,					// Device (preferred) Frame Size (RT Audio will adjust this)
//			&PrimaryAudioCallback,					// Audio Callback
//			_configuration,							// SynthConfiguration* Shared Pointer***
//			&options);						
//
//		// Initialize Playback Stream
//		_outputStream = new PlaybackBuffer<float>(2,
//			outputDevice.preferredSampleRate,
//			outputBufferFrameSize,
//			8,
//			SIGNAL_LOW,
//			SIGNAL_HIGH);
//
//		_rtOutputDevice = outputDevice;
//
//		//std::string deviceFormat("Float32 - Channels");
//
//		const char* deviceFormat = "Bizzare Unknown";
//
//		// Store output parameters
//		_outputParameters = new PlaybackParameters(
//			_rtAudio->getApiDisplayName(_rtAudio->getCurrentApi()).c_str(),
//			deviceFormat,
//			_rtOutputDevice.name.c_str(),
//			_rtOutputDevice.preferredSampleRate, 
//			_rtOutputDevice.outputChannels, 
//			outputBufferFrameSize);
//	}
//	catch (std::exception ex)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool DisposeAudioClient()
//{
//	try
//	{
//		if (_rtAudio == NULL)
//			return true;
//
//		if (_rtAudio->isStreamRunning())
//			_rtAudio->stopStream();
//
//		if (_rtAudio->isStreamOpen())
//			_rtAudio->closeStream();
//
//		delete _rtAudio;
//		delete _outputStream;
//		delete _synthDevice;
//		delete _configuration;
//		delete _streamClock;
//		delete _uiTimer;
//		delete _audioTimer;
//
//		_rtAudio = NULL;
//		_outputStream = NULL;
//		_synthDevice = NULL;
//		_configuration = NULL;
//		_streamClock = NULL;
//		_uiTimer = NULL;
//		_audioTimer = NULL;
//		_synthIntervalTimer = NULL;
//	}
//	catch (std::exception ex)
//	{
//		return false;
//	}
//
//	return true;
//}
//
///// <summary>
///// Initialization function for the synth backend. This must be called before starting the player!
///// </summary>
//void InitializePlayback()
//{
//	_configuration = new SynthConfiguration();
//	_synthDevice = new SynthPlaybackDevice<float>();
//	_streamClock = new PlaybackClock();
//	_uiTimer = new LoopTimer(0.075);
//	_audioTimer = new LoopTimer(_outputParameters->GetOutputBufferFrameSize() / (double)_outputParameters->GetSamplingRate());
//	_synthIntervalTimer = new IntervalTimer();
//
//	// Octave 1
//	_configuration->SetMidiNote(WindowsKeyCodes::Z, 21);
//	_configuration->SetMidiNote(WindowsKeyCodes::X, 22);
//	_configuration->SetMidiNote(WindowsKeyCodes::C, 23);
//	_configuration->SetMidiNote(WindowsKeyCodes::V, 24);
//	_configuration->SetMidiNote(WindowsKeyCodes::B, 25);
//	_configuration->SetMidiNote(WindowsKeyCodes::N, 26);
//	_configuration->SetMidiNote(WindowsKeyCodes::M, 27);
//	_configuration->SetMidiNote(WindowsKeyCodes::COMMA, 28);
//	_configuration->SetMidiNote(WindowsKeyCodes::PERIOD, 29);
//	_configuration->SetMidiNote(WindowsKeyCodes::QUESTION_MARK, 30);
//
//	// Octave 2
//	_configuration->SetMidiNote(WindowsKeyCodes::A, 33);
//	_configuration->SetMidiNote(WindowsKeyCodes::S, 34);
//	_configuration->SetMidiNote(WindowsKeyCodes::D, 35);
//	_configuration->SetMidiNote(WindowsKeyCodes::F, 36);
//	_configuration->SetMidiNote(WindowsKeyCodes::G, 37);
//	_configuration->SetMidiNote(WindowsKeyCodes::H, 38);
//	_configuration->SetMidiNote(WindowsKeyCodes::J, 39);
//	_configuration->SetMidiNote(WindowsKeyCodes::K, 40);
//	_configuration->SetMidiNote(WindowsKeyCodes::L, 41);
//	_configuration->SetMidiNote(WindowsKeyCodes::SEMICOLON, 42);
//	_configuration->SetMidiNote(WindowsKeyCodes::APOSTROPHE, 43);
//
//	// Octave 3
//	_configuration->SetMidiNote(WindowsKeyCodes::Q, 44);
//	_configuration->SetMidiNote(WindowsKeyCodes::W, 45);
//	_configuration->SetMidiNote(WindowsKeyCodes::E, 46);
//	_configuration->SetMidiNote(WindowsKeyCodes::R, 47);
//	_configuration->SetMidiNote(WindowsKeyCodes::T, 48);
//	_configuration->SetMidiNote(WindowsKeyCodes::Y, 49);
//	_configuration->SetMidiNote(WindowsKeyCodes::U, 50);
//	_configuration->SetMidiNote(WindowsKeyCodes::I, 51);
//	_configuration->SetMidiNote(WindowsKeyCodes::O, 52);
//	_configuration->SetMidiNote(WindowsKeyCodes::P, 53);
//	_configuration->SetMidiNote(WindowsKeyCodes::LEFT_BRACKET, 54);
//	_configuration->SetMidiNote(WindowsKeyCodes::RIGHT_BRACKET, 55);
//
//	// Octave 4
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_1, 56);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_2, 57);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_3, 58);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_4, 59);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_5, 60);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_6, 61);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_7, 62);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_8, 63);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_9, 64);
//	_configuration->SetMidiNote(WindowsKeyCodes::NUMBER_0, 65);
//	_configuration->SetMidiNote(WindowsKeyCodes::MINUS, 66);
//	_configuration->SetMidiNote(WindowsKeyCodes::PLUS, 67);
//
//	_synthDevice->Initialize(_configuration, *_outputParameters);
//	_rtAudio->startStream();
//}
//
//void LoopUI()
//{
//	// Reset All Timers
//	_streamClock->Start();
//	_streamClock->Mark();
//	_uiTimer->Reset();
//	_audioTimer->Reset();
//
//	bool isUIDirty = false;
//
//	// FTX-UI (Terminal Loop / Renderer)
//	// 
//	// Their backend will handle interaction, resizing, and redrawing. The document should be
//	// updated on this thread (AFAIK) which is captured in the lambda function(s). 
//	// 
//	// After more working with FTXUI, it is using std::move, and std::make_shared, and std::shared_ptr
//	// to try to keep stack allocations, and move them to the heap. I'm not as familiar with this pattern;
//	// but it is very difficult to reproduce an inheritance pattern. Though, not impossible, it would
//	// be a better use of time to just leave it on our stack, which will accomplish the same task. 
//	// 
//	// There is a slight memory leak which may be in FTXUI; but I'm not sure, yet, if RT Audio has issues,
//	// or just our SynthNote* allocations (probably not).
//	//
//	// https://arthursonzogni.github.io/FTXUI/
//	//
//
//	auto screen = ftxui::ScreenInteractive::TerminalOutput();
//
//	// Source Oscillator
//	OscillatorUI oscillatorUI("Oscillator", true, ftxui::Color::Blue);
//
//	// Note Envelope
//	EnvelopeUI envelopeUI(_configuration->GetNoteEnvelope(), "Input Envelope", true, ftxui::Color::BlueLight);
//
//	// Envelope Filter (Oscillator)
//	EnvelopeFilterUI envelopeFilterUI(_configuration->GetEnvelopeFilterType(),
//		_configuration->GetEnvelopeFilterOscillatorType(),
//		_configuration->GetEnvelopeFilter(),
//		_configuration->GetEnvelopeFilterCutoff(),
//		_configuration->GetEnvelopeFilterResonance(),
//		_configuration->GetEnvelopeFilterOscillatorFrequency(),
//		_configuration->GetHasEnvelopeFilter(),
//		"Envelope Filter", ftxui::Color::Red);
//	// Reverb
//	ReverbUI reverbUI(_configuration->GetHasReverb(),
//		_configuration->GetReverbDelaySeconds(),
//		_configuration->GetReverbGain(),
//		_configuration->GetReverbWetDry(),
//		"Reverb", ftxui::Color::Green);
//
//	// Delay
//	DelayUI delayUI(_configuration->GetHasDelay(),
//		_configuration->GetDelayFeedback(),
//		_configuration->GetDelaySeconds(),
//		_configuration->GetDelayGain(),
//		_configuration->GetDelayWetDry(),
//		"Delay", ftxui::Color::Purple);
//
//	// Compressor
//	CompressorUI compressorUI(_configuration->GetHasCompressor(),
//		_configuration->GetCompressorThreshold(),
//		_configuration->GetCompressorGain(),
//		_configuration->GetCompressorAttack(),
//		_configuration->GetCompressorRelease(),
//		_configuration->GetCompressionRatio(),
//		"Compressor", ftxui::Color::HotPink);
//
//	// Output
//	OutputUI outputUI(_configuration->GetOutputGain(), _configuration->GetOutputLeftRight(), "Output", ftxui::Color::Green);
//
//	auto synthInputSettings = ftxui::Container::Horizontal({
//
//		oscillatorUI.GetComponent() | ftxui::flex | ftxui::border,
//		envelopeUI.GetComponent() | ftxui::flex | ftxui::border,
//		envelopeFilterUI.GetComponent() | ftxui::flex | ftxui::border
//
//	}) | ftxui::flex_grow;
//
//	auto synthEffectsSettings = ftxui::Container::Horizontal({
//
//		compressorUI.GetComponent() | ftxui::flex_grow | ftxui::border,
//		reverbUI.GetComponent() | ftxui::flex_grow | ftxui::border,
//		delayUI.GetComponent() | ftxui::flex_grow | ftxui::border
//
//	});
//
//	auto synthOutputSettings = ftxui::Container::Horizontal({
//
//		outputUI.GetComponent() | ftxui::flex_grow | ftxui::border,
//
//	});
//
//	auto synthSettings = ftxui::Container::Vertical({
//		synthInputSettings | ftxui::flex_grow,
//		synthEffectsSettings | ftxui::flex_grow,
//		synthOutputSettings | ftxui::flex_grow,
//	});
//
//	// UI BACKEND LOOP!! This will be run just for re-drawing purposes during our
//	//					 primary loop below.
//	//
//	auto renderer = ftxui::Renderer(synthSettings, [&]
//	{
//		// Terminal Synth Information
//		auto synthInformation = ftxui::hbox(
//		{
//			ftxui::vbox(
//			{
//				ftxui::text("Host API:                 " + _outputParameters->GetHostApi()),
//				ftxui::text("Device Name:              " + _outputParameters->GetDeviceName()),
//				ftxui::text("Stream Format:            " + _outputParameters->GetDeviceFormat()),
//				ftxui::text("Stream Buffer Size:       " + std::format("{} (frames)", _outputParameters->GetOutputBufferFrameSize())),
//				ftxui::text("Sample Rate (Hz):         " + std::to_string(_outputParameters->GetSamplingRate()))
//
//			}) | ftxui::flex_grow,
//
//			ftxui::separator(),
//
//			ftxui::vbox(
//			{
//				ftxui::text(" Stream Time            (s):    " + std::format("{:.3f}", _streamClock->GetTime())),
//				ftxui::text(" Avg. UI Time          (ms):    " + std::format("{:.3f}", _uiTimer->GetAvgMilli())),
//				ftxui::text(" Avg. Callback Time    (ms):    " + std::format("{:.3f}", _audioTimer->GetAvgMilli())),
//				ftxui::text(" Avg. Frontend Time    (ms):    " + std::format("{:.3f}", _synthIntervalTimer->AvgMilli())),
//				ftxui::text(" Stream Latency   (samples):    " + std::to_string(_rtAudio->getStreamLatency()))
//
//			}) | ftxui::flex_grow
//		}) | ftxui::flex_grow;
//
//
//		// Borderless Layout Grid
//		auto container = ftxui::vbox({
//
//			ftxui::vbox({
//				ftxui::text("Terminal Synth") | ftxui::color(ftxui::Color(0,255,0,255)),
//				ftxui::separator(),
//				synthInformation,
//
//			}) | ftxui::border | ftxui::flex_grow,
//
//			synthSettings->Render() | ftxui::border | ftxui::flex_grow
//
//		}) | ftxui::flex_grow;
//
//		return container;
//
//	}) | ftxui::CatchEvent([&](ftxui::Event event) {
//
//		// Only allow mouse events through
//		if (event.mouse().button == ftxui::Mouse::Left)
//		{
//			isUIDirty |= event.is_mouse();
//			return false;
//		}
//
//		// Cancel keyboard events
//		return true;
//	});
//
//
//	// Initialize Screen (sizing)
//	screen.FitComponent();
//
//	// FTXUI has an option to create an event loop (this will run their backend UI code)
//	//
//	// https://arthursonzogni.com/FTXUI/doc/examples_2component_2custom_loop_8cpp-example.html#_a8
//	//
//	ftxui::Loop loop(& screen, renderer);
//
//	// Primary Loop!!! We'll handle this loop - using our system timer to manage the 
//	//				   accuracy of the audio output; and also throttle events for the
//	//				   UI's backend. We should be redrawing ~10ms
//	while (!loop.HasQuitted())
//	{
//		if (GetAsyncKeyState(VK_ESCAPE))
//		{
//			screen.ExitLoopClosure();
//			break;
//		}
//
//		// Synth Configuration:  Our copy is updated on the UI Update timer
//		//
//		if (_uiTimer->Mark())
//		{
//			// Force timer to wait until next goal is reached
//			//
//			_uiTimer->Reset();
//
//			// These were added to help create UI classes. The stack-oriented rendering
//			// architecture of FTXUI is tricky to get to provide an update each call. You
//			// basically have to either follow their UI inheritance pattern (closely), or
//			// you have to add something to trigger re-rendering!
//			oscillatorUI.UpdateComponent(true);
//			envelopeUI.UpdateComponent(true);
//			envelopeFilterUI.UpdateComponent(true);
//
//			delayUI.UpdateComponent(true);
//			reverbUI.UpdateComponent(true);
//			compressorUI.UpdateComponent(true);
//
//			outputUI.SetOutput(_outputL, _outputR);
//			outputUI.UpdateComponent(true);
//
//			// Use custom event to force one UI update
//			screen.PostEvent(ftxui::Event::Custom);
//
//			// Update Configuration
//			if (isUIDirty)
//			{
//				isUIDirty = false;
//
//				// Enable
//				_configuration->SetHasEnvelopeFilter(envelopeFilterUI.GetEnabled());
//				_configuration->SetHasDelay(delayUI.GetEnabled());
//				_configuration->SetHasCompressor(compressorUI.GetEnabled());
//				_configuration->SetHasReverb(reverbUI.GetEnabled());
//				
//				// Oscillator
//				_configuration->SetOscillatorType(oscillatorUI.GetSelection());
//
//				// Envelope
//				_configuration->SetNoteEnvelope(envelopeUI.GetSelection());
//
//				// Envelope Filter
//				_configuration->SetEnvelopeFilter(envelopeFilterUI.GetEnvelope());
//				_configuration->SetEnvelopeFilterType(envelopeFilterUI.GetType());
//				_configuration->SetEnvelopeFilterCutoff(envelopeFilterUI.GetCutoff());
//				_configuration->SetEnvelopeFilterResonance(envelopeFilterUI.GetResonance());
//				_configuration->SetEnvelopeFilterOscillatorFrequency(envelopeFilterUI.GetOscillatorFrequency());
//				_configuration->SetEnvelopeFilterOscillatorType(envelopeFilterUI.GetOscillatorType());
//
//				// Reverb
//				_configuration->SetReverbSeconds(reverbUI.GetDelay());
//				_configuration->SetReverbGain(reverbUI.GetGain());
//				_configuration->SetReverbWetDry(reverbUI.GetWetDry());
//
//				// Delay
//				_configuration->SetDelayFeedback(delayUI.GetFeedbackEnabled());
//				_configuration->SetDelaySeconds(delayUI.GetDelay());
//				_configuration->SetDelayGain(delayUI.GetGain());
//				_configuration->SetDelayWetDry(delayUI.GetWetDry());
//
//				// Compressor
//				_configuration->SetCompressionRatio(compressorUI.GetCompressionRatio());
//				_configuration->SetCompressorAttack(compressorUI.GetAttack());
//				_configuration->SetCompressorGain(compressorUI.GetGain());
//				_configuration->SetCompressorRelease(compressorUI.GetRelease());
//				_configuration->SetCompressorThreshold(compressorUI.GetThreshold());
//
//				// Output
//				_configuration->SetOutputGain(outputUI.GetGain());
//				_configuration->SetOutputLeftRight(outputUI.GetLeftRight());
//			}
//
//			// UI Run
//			loop.RunOnce();
//		}
//
//		// Only update if changes were made (~100ms)
//		if (_configuration->IsDirty())
//		{
//			// CRITICAL SECTION:  This is an update from the UI, which will reset the synth parameters. So,
//			//					  it is only allowed every ~100ms at the most.
//			//
//
//			_rtAudio->stopStream();
//
//			_synthDevice->UpdateSynth(_configuration);
//
//			// This timer is not thread-safe; but the usage seems to be ok on both threads
//			// without stopping the audio stream. We need to re-measure the metrics up-to-date.
//			_synthIntervalTimer->ClearAll();
//			_streamClock->Reset();
//			_uiTimer->Reset();
//			_audioTimer->Reset();
//
//			_rtAudio->startStream();
//
//			// Reset Configuration Flag
//			_configuration->ClearDirty();
//		}
//		
//		std::this_thread::sleep_for(std::chrono::microseconds(100));
//	}
//}


int main(int argc, char* argv[], char* envp[])
{
	//SetConsoleTitleA("Terminal Synth");

	//// Read midi file
	//if (argc > 1)
	//{
	//}

	//// Manual keyboard input
	//else
	//{
	//	if (!InitializeAudioClient())
	//		return -1;

	//	InitializePlayback();
	//}

	//LoopUI();

	//if (!DisposeAudioClient())
	//	return -1;

	return 0;
}