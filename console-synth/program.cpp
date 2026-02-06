#include "Constant.h"
#include "Envelope.h"
#include "LoopTimer.h"
#include "PlaybackClock.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"
#include "WindowsKeyCodes.h"
#include <Windows.h>
#include <portaudio.h>
//#include <chrono>
//#include <exception>
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
#include "PlaybackBuffer.h"
#include <exception>


// Static Instance (RT Audio) (Couldn't figure out how to cast function callbacks to the framework)
PaStream* _audioStream;
PlaybackBuffer<float>* _outputStream;
SynthPlaybackDevice<float>* _synthDevice;
SynthConfiguration* _configuration;
PlaybackClock* _streamClock;
PaTime _startTime;
PaTime _writeTime;

void HandleError(PaError error)
{
	switch (error)
	{
	case paNoError:
		break;
	case paNotInitialized:
		throw new std::exception("Port Audio Error:  Not Initialized");
	case paUnanticipatedHostError:
		throw new std::exception("Port Audio Error:  Unanticipated Host (check host configuration)");
	case paInvalidChannelCount:
		throw new std::exception("Port Audio Error:  Invalid Channel Count");
	case paInvalidSampleRate:
		throw new std::exception("Port Audio Error:  Invalid Sample Rate");
	case paInvalidDevice:
		throw new std::exception("Port Audio Error:  Invalid Device");
	case paInvalidFlag:
		throw new std::exception("Port Audio Error:  Invalid Flag");
	case paSampleFormatNotSupported:
		throw new std::exception("Port Audio Error:  Sample Format Not Supported");
	case paBadIODeviceCombination:
		throw new std::exception("Port Audio Error:  Invalid IO Device Combination (check host configuration)");
	case paInsufficientMemory:
		throw new std::exception("Port Audio Error:  Insufficient Memory");
	case paBufferTooBig:
		throw new std::exception("Port Audio Error:  Buffer too big!");
	case paBufferTooSmall:
		throw new std::exception("Port Audio Error:  Buffer too small!");
	case paNullCallback:
		throw new std::exception("Port Audio Error:  Null Callback Error (check blocking configuration)");
	case paBadStreamPtr:
		throw new std::exception("Port Audio Error:  Bad stream pointer");
	case paTimedOut:
		throw new std::exception("Port Audio Error:  Timed out");
	case paInternalError:
		throw new std::exception("Port Audio Error:  Internal Error");
	case paDeviceUnavailable:
		throw new std::exception("Port Audio Error:  Device Unavailalbe");
	case paIncompatibleHostApiSpecificStreamInfo:
		throw new std::exception("Port Audio Error:  Incompatible Host API (check configuration)");
	case paStreamIsStopped:
		throw new std::exception("Port Audio Error:  Stream has stopped");
	case paStreamIsNotStopped:
		throw new std::exception("Port Audio Error:  Stream has not stopped. Call Pa_StopStream first!");
	case paInputOverflowed:
		throw new std::exception("Port Audio Error:  Input overflow");
	case paOutputUnderflowed:
		//throw new std::exception("Port Audio Error:  Output underflow");
		break;
	case paHostApiNotFound:
		throw new std::exception("Port Audio Error:  Host API Not found");
	case paInvalidHostApi:
		throw new std::exception("Port Audio Error:  Invalid Host API");
	case paCanNotReadFromACallbackStream:
		throw new std::exception("Port Audio Error:  Cannot read from callback stream");
	case paCanNotWriteToACallbackStream:
		throw new std::exception("Port Audio Error:  Cannot write to callback stream");
	case paCanNotReadFromAnOutputOnlyStream:
		throw new std::exception("Port Audio Error:  Cannot read from output only stream");
	case paCanNotWriteToAnInputOnlyStream:
		throw new std::exception("Port Audio Error:  Cannot write to input only stream");
	case paIncompatibleStreamHostApi:
		throw new std::exception("Port Audio Error:  Incompatible stream host API");
	case paBadBufferPtr:
		throw new std::exception("Port Audio Error:  Bad Buffer Pointer");
	case paCanNotInitializeRecursively:
		throw new std::exception("Port Audio Error:  Cannot initialize recursively");
	default:
		throw new std::exception("Unhandled Port Audio Error:  program.cpp");
	}
}

bool InitializePortAudio()
{
	// Port Audio: RT Audio dis-continued their blocking callback (single threaded functionality)
	//			   We need to be able to update the synth possibly on each tick. So, using locks
	//			   may have degraded audio quality and caused glitches.
	// 
	// https://files.portaudio.com/docs/v19-doxydocs/tutorial_start.html
	//

	// Initialize Port Audio
	PaError error = Pa_Initialize();

	if (error != PaErrorCode::paNoError)
	{
		HandleError(error);
		return false;
	}

	// WASAPI Device
	PaHostApiIndex wasapiIndex = Pa_HostApiTypeIdToHostApiIndex(PaHostApiTypeId::paMME);
	PaDeviceIndex outputDeviceIndex;

	for (PaDeviceIndex index = 0; index < Pa_GetDeviceCount(); index++)
	{
		const PaDeviceInfo* device = Pa_GetDeviceInfo(index);

		if (device->hostApi == wasapiIndex)
		{
			outputDeviceIndex = index;
		}
	}

	// Output Parameters
	PaStreamParameters outputParameters;	

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = NUMBER_CHANNELS;
	outputParameters.sampleFormat = paFloat32;	
	outputParameters.hostApiSpecificStreamInfo = NULL;

	// Output Device Info
	const PaDeviceInfo* outputDeviceInfo = Pa_GetDeviceInfo(outputParameters.device);
	outputParameters.suggestedLatency = outputDeviceInfo->defaultLowOutputLatency;
	//outputParameters.suggestedLatency = 0;

	// Stream Flags:  Must set these up for variable streaming!
	PaStreamFlags streamFlags = paClipOff | paDitherOff;

	/* -- setup stream -- */
	error = Pa_OpenStream(
			&_audioStream,
			NULL,
			&outputParameters,
			SAMPLING_RATE,
			paFramesPerBufferUnspecified,	/* the stream will accept a variable number of frames */
			streamFlags,					/* we won't output out of range samples so don't bother clipping them */
			NULL,							/* no callback, use blocking API */
			NULL);							/* no callback, so no callback userData */

	if (error != PaErrorCode::paNoError)
	{
		HandleError(error);
		return false;
	}

	error = Pa_StartStream(_audioStream);

	if (error != PaErrorCode::paNoError)
	{
		HandleError(error);
		return false;
	}

	// Initialize Playback Stream
	_outputStream = new PlaybackBuffer<float>(outputParameters.channelCount, 
											  SAMPLING_RATE, 
											  AUDIO_BUFFER_SIZE * 1000, 
											  SIGNAL_LOW, 
											  SIGNAL_HIGH);

	_streamClock = new PlaybackClock();
	_streamClock->Start();

	_startTime = Pa_GetStreamTime(_audioStream);

	return true;
}

bool DisposePortAudio()
{
	PaError error = Pa_StopStream(_audioStream);	

	if (error != PaErrorCode::paNoError)
	{
		HandleError(error);
		return false;
	}

	error = Pa_CloseStream(_audioStream);

	if (error != PaErrorCode::paNoError)
	{
		HandleError(error);
		return false;
	}

	return true;
}

bool WritePlaybackStream()
{
	// Procedure
	//
	// 1) Get length of time since last write
	// 2) Get Port Audio stream time
	// 3) Request that amount of frames filled in the playback stream
	// 4) Write that stream to Port Audio
	//

	// Stream Time:  This is a real time parameter which carries the approximate stream tick. The
	//				 accuracy of this is left to Port Audio; but we're going to keep counters of 
	//				 any small latencies that develop.
	
	//double currentTime = _streamClock->GetDelta();

	//auto latency = Pa_GetStreamInfo(_audioStream);

	// Calculate end frame index. Write from the beginning to the specified frame.
	//int nextFrameSize = (_streamClock->GetTime() - _streamClock->GetMarkTime()) * SAMPLING_RATE;

	// Port Audio:  Ready for N frames
	//long framesAvailable = Pa_GetStreamWriteAvailable(_audioStream);

	//if (framesAvailable < nextFrameSize)
	//{
	//	int foo = 3;
	//	return true;
	//}

	// First Stream Check
	if (_writeTime == 0)
	{
		_writeTime = Pa_GetStreamTime(_audioStream) - _startTime;
		return true;
	}
		
	auto numberOfFrames = Pa_GetStreamWriteAvailable(_audioStream);

	if (numberOfFrames <= 0)
	{
		return true;
	}


	PaTime updateTime = Pa_GetStreamTime(_audioStream) - _startTime;

	// Port Audio:  Next stream frame count using PaTime from their backend
	//
	//unsigned int numberOfFrames = (((updateTime - _writeTime)) * SAMPLING_RATE) + 1;

	// Playback Device: Creates output samples for the stream based on the current stream time.
	//
	_synthDevice->WritePlaybackBuffer(_outputStream, numberOfFrames, updateTime);

	// Port Audio:  Writes the stream to the output device
	//
	PaError error = Pa_WriteStream(_audioStream, _outputStream->GetBuffer(), numberOfFrames);

	if (error != PaErrorCode::paNoError)
	{
		HandleError(error);
		return false;
	}

	// Updates last marker for measuring delta time
	//
	_streamClock->Mark();
	_writeTime = updateTime;

	return true;
}

/// <summary>
/// Initialization function for the synth backend. This must be called before starting the player!
/// </summary>
void InitializePlayback()
{
	_configuration = new SynthConfiguration();
	_synthDevice = new SynthPlaybackDevice<float>();

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

	_synthDevice->Initialize();
}

void LoopUI()
{
	LoopTimer uiTimer(0.001), audioTimer(0.0001);
	bool exit = false;

	// Port Audio Variables
	std::string version = Pa_GetVersionInfo()->versionText;
	std::string hostApi = Pa_GetHostApiInfo(Pa_GetDefaultHostApi())->name;
	double streamLatency = 0;

	// FTX-UI (Terminal Loop / Renderer)
	// 
	// Their backend will handle interaction, resizing, and redrawing. The document should be
	// updated on this thread (AFAIK) which is captured in the lambda function.
	//
	// https://arthursonzogni.github.io/FTXUI/
	//

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

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
	float filterCutoffMax = MAX_FREQUENCY;
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

	auto oscillatorStrs = std::vector<std::string>({
		"Sine",
		"Square",
		"Triangle",
		"Sawtooth",
		"Random"
	});
	auto onOffStrs = std::vector<std::string>({
		"On",
		"Off"
	});
	auto envelopeFilterTypeStrs = std::vector<std::string>({
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
	auto envelopeFilterLabelUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text("Envelope Filter") | ftxui::color(ftxui::Color(255,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &envelopeFilterEnabled) | ftxui::align_right,
	});

	// Envelope Filter Type Label (Toggle)
	auto envelopeFilterTypeLabelUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text("Filter Type") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(envelopeFilterTypeStrs, &envelopeFilterTypeChoice) | ftxui::align_right,
	});

	// Envelope Filter (Oscillator)
	auto envelopeOscillatorUI = ftxui::Container::Vertical({
		ftxui::Renderer([&] { return ftxui::text("Oscillator Type (VCO)"); }),
		ftxui::Renderer([&] { return ftxui::separator(); }),
		ftxui::Radiobox(&oscillatorStrs, &envelopeOscillatorChoice),
		ftxui::Renderer([&] { return ftxui::separator(); }),
		ftxui::Slider(&filterOscillatorStr, &filterOscillatorFrequency, filterOscillatorFrequencyLow, filterOscillatorFrequencyHigh, filterOscillatorFrequencyIncr),
	});

	// Envelope Filter (Sweep)
	auto envelopeSweepUI = ftxui::Container::Vertical({
		ftxui::Slider(&filterAttackStr, &filterAttack, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider(&filterSustainStr, &filterSustain, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider(&filterDecayStr, &filterDecay, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider(&filterReleaseStr, &filterRelease, envelopeMin, envelopeMax, envelopeIncrement),
	});

	// Reverb (Enable)
	auto reverbEnableUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text("Reverb") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &reverbEnabled) | ftxui::align_right,

	});

	// Reverb
	auto reverbUI = ftxui::Container::Vertical({

		reverbEnableUI,
		ftxui::Renderer([&] { return ftxui::separator(); }),

		// Delay (s)
		ftxui::Slider(&reverbDelayStr, &reverbDelaySeconds, reverbDelayMin, reverbDelayMax, reverbDelayIncr),

		// Gain
		ftxui::Slider(&reverbGainStr, &reverbGain, lineMin, lineMax, lineIncr),
	});

	// Delay (Enable)
	auto delayEnableUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text("Delay") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &delayEnabled) | ftxui::align_right,

	});

	// Delay Feedback Enable (Toggle)
	auto delayFeedbackUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text("Feedback") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &delayFeedbackEnabled) | ftxui::align_right,

	});

	// Delay
	auto delayUI = ftxui::Container::Vertical({

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
	auto compressorEnableUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text("Compressor") | ftxui::color(ftxui::Color(0,0,255,255)); }) | ftxui::flex_grow,
		ftxui::Toggle(onOffStrs, &compressorEnabled) | ftxui::align_right,

	});

	// Compressor
	auto compressorUI = ftxui::Container::Vertical({

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
	auto envelopeTypeUI = ftxui::Container::Vertical({

		// Manual 
		ftxui::Renderer([&] { return ftxui::text("Manual Settings"); }) | ftxui::Maybe([&] { return envelopeFilterTypeChoice == 0; }),

		// Oscillator
		envelopeOscillatorUI | ftxui::Maybe([&]{ return envelopeFilterTypeChoice == 1; }),

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
	auto reverbUIRenderer = ftxui::Renderer(reverbUI, [&] {

		reverbDelayStr = "Delay (s) " + std::format("{:.4f}", reverbDelaySeconds);
		reverbGainStr = "Gain      " + std::format("{:.2f}", reverbGain) + "  ";

		return reverbUI->Render();
	});

	// Delay
	auto delayUIRenderer = ftxui::Renderer(delayUI, [&] {

		delayStr = "Delay (s) " + std::format("{:.2f}", delaySeconds);
		delayGainStr = "Gain      " + std::format("{:.2f}", delayGain);

		return delayUI->Render();
	});

	// Compressor
	auto compressorUIRenderer = ftxui::Renderer(compressorUI, [&] {

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
		// Synth Information
		auto synthInformation = ftxui::vbox(
		{
			ftxui::text(version),
			ftxui::text("Host API:				   " + hostApi),
			ftxui::text("Current Time     (s):     " + std::format("{:.3f}", _streamClock->GetTime())),
			ftxui::text("Avg. UI Time     (ms):    " + std::format("{:.3f}", uiTimer.GetAvgMilli())),
			ftxui::text("Avg. Sample Time (ms):    " + std::format("{:.3f}", audioTimer.GetAvgMilli())),
			ftxui::text("Stream Latency   (ms):    " + std::format("{:.3f}", streamLatency)),
			ftxui::text("Sample Rate      (Hz):    " + std::to_string(SAMPLING_RATE)),
			ftxui::text("Frame Period     (ms):    " + std::to_string((_outputStream->GetMaxNumberOfFrames() / (double)SAMPLING_RATE) * 1000)),

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

		//double currentTime = _streamClock->GetTime();
		double currentTime = Pa_GetStreamTime(_audioStream) - _startTime;

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
				_synthDevice->SetNote(midiNote, false, currentTime);

			// Engage
			else if (!_synthDevice->HasNote(midiNote) && isPressed)
				_synthDevice->SetNote(midiNote, true, currentTime);
		}

		// Clean Up Synth Notes
		_synthDevice->ClearUnused(currentTime);

		// Synth Configuration:  Our copy is updated on the UI Update timer
		//
		if (uiTimer.Mark())
		{
			// Force timer to wait until next goal is reached
			//
			uiTimer.Reset();

			// Use custom event to force one UI update
			screen.PostEvent(ftxui::Event::Custom);

			// Update Configuration
			_configuration->SetNoteEnvelope(Envelope(noteAttack, noteDecay, noteSustain, noteRelease, envelope.GetAttackPeak(), envelope.GetSustainPeak()));
			_configuration->SetEnvelopeFilter(Envelope(filterAttack, filterDecay, filterSustain, filterRelease, filter.GetAttackPeak(), filter.GetSustainPeak()));
			_configuration->SetOscillatorType((AmplitudeOscillatorType)oscillatorChoice);
			_configuration->SetEnvelopeFilterType((EnvelopeFilterType)envelopeFilterTypeChoice);
			_configuration->SetEnvelopeFilterCutoff(filterCutoff);
			_configuration->SetEnvelopeFilterResonance(filterResonance);
			_configuration->SetEnvelopeFilterOscillatorFrequency(filterOscillatorFrequency);
			_configuration->SetEnvelopeFilterOscillatorType((AmplitudeOscillatorType)envelopeOscillatorChoice);
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

			//_player->StopStream();

			_synthDevice->UpdateSynth(*_configuration);

			//_player->StartStream();

			// Reset Configuration Flag
			_configuration->ClearDirty();
		}
		
		// PA Audio Callback (N Frames) (in micro seconds)
		//if (lastAudioUpdate >= (((_outputStream->GetNumberOfFrames() / (double)SAMPLING_RATE))) * 1000)
		//{

		audioTimer.Mark();

			bool streamOk = WritePlaybackStream();

			// PA ERROR
			if (!streamOk)
				screen.ExitLoopClosure();
		//}
	

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));

		//_lock->unlock();
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
		InitializePortAudio();
		InitializePlayback();
	}

	LoopUI();

	DisposePortAudio();

	return 0;
}
