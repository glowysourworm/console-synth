#include "Constant.h"
#include "Envelope.h"
#include "RtAudio.h"
#include "StopWatch.h"
#include "SynthConfiguration.h"
#include "SynthPlaybackDevice.h"
#include "SynthPlayer.h"
#include "WindowsKeyCodes.h"
#include <Windows.h>
#include <chrono>
#include <cstdio>
#include <exception>
#include <format>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/direction.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <vector>


// Static Instance (RT Audio) (Couldn't figure out how to cast function callbacks to the framework)
SynthPlayer* _player;
SynthConfiguration* _configuration;
std::mutex* _lock;

/// <summary>
/// Primary RT Audio callback - which happens on RT Audio's thread!!!  
/// </summary>
int PrimaryAudioCallback(void* outputBuffer,
	void* inputBuffer,
	unsigned int nFrames,
	double streamTime,
	RtAudioStreamStatus status,
	void* userData)
{
	_lock->lock();

	int returnValue = _player->GetDevice()->RtAudioCallback(outputBuffer, inputBuffer, nFrames, streamTime, status, userData);

	_lock->unlock();

	return returnValue;
}

void PrimaryErrorCallback(RtAudioError::Type type, const std::string& message)
{

}

/// <summary>
/// Initialization function for the synth backend. This must be called before starting the player!
/// </summary>
void Initialize()
{
	_configuration = new SynthConfiguration();
	_player = new SynthPlayer();

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

	_player->Initialize(&PrimaryAudioCallback, &PrimaryErrorCallback);
}

/// <summary>
/// Returns true if the key should be processed by FTXUI
/// </summary>
bool HandleKeysPressed(const std::string& ftxuiCharacters)
{
	// Favor the synth notes for keys pressed
	bool synthMapped = false;

	_lock->lock();

	// WINAPI (Need to get the character's pressed to translate back to the key codes entered)
	//
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-vkkeyscanexa?redirectedfrom=MSDN
	//
	for (int index = 0; index < ftxuiCharacters.size(); index++)
	{
		// Translate character to key code
		WindowsKeyCodes keyCode = (WindowsKeyCodes)VkKeyScanA(ftxuiCharacters[index]);

		if (!_configuration->HasMidiNote(keyCode))
			continue;

		synthMapped = true;

		// Get Midi Note from configuration
		int midiNote = _configuration->GetMidiNote(keyCode);

		// Set New Note! (MEMORY!)
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(midiNote, true, _player->GetStreamTime());
	}

	// Unset Active Notes
	int midiNotes[MIDI_PIANO_SIZE];
	int midiNotesLength = 0;

	((SynthPlaybackDevice*)_player->GetDevice())->GetNotes(midiNotes, midiNotesLength);

	// Iterate small array of midi notes to see what notes aren't being pressed
	for (int index = 0; index < midiNotesLength; index++)
	{
		// Get key code for this midi note
		WindowsKeyCodes keyCode = _configuration->GetKeyCode(midiNotes[index]);

		// WINAPI map key code back to character
		char character = MapVirtualKeyA((int)keyCode, MAPVK_VK_TO_CHAR);

		// Unset Note!
		if (!ftxuiCharacters.contains(character))
			((SynthPlaybackDevice*)_player->GetDevice())->SetNote(midiNotes[index], false, _player->GetStreamTime());
	}

	// Clear out any unused notes (MEMORY!)
	((SynthPlaybackDevice*)_player->GetDevice())->ClearUnused(_player->GetStreamTime());

	_lock->unlock();

	return synthMapped;
}

void LoopUI()
{
	StopWatch uiStopWatch, audioStopWatch;
	bool exit = false;
	int lastUIUpdate = 0;

	// Initialize Stop Watches
	audioStopWatch.mark();
	uiStopWatch.mark();

	// Get Current Time
	double systemTime = 0;
	double streamLatency = 0;
	double averageAudioTime = 0;

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

	float envelopeMin = 0.01f;
	float envelopeMax = 3.0f;
	float envelopeIncrement = 0.01f;

	float filterCutoffMin = 60;
	float filterCutoffMax = MAX_FREQUENCY;
	float filterCutoffIncr = 10;

	float filterResonanceMin = 0.0f;
	float filterResonanceMax = 1.0f;
	float filterResonanceIncr = 0.01f;

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

	int oscillatorChoice = (int)_configuration->GetOscillatorType();
	int envelopeFilterEnabled = _configuration->GetHasEnvelopeFilter() ? 0 : 1;
	int envelopeFilterTypeChoice = (int)_configuration->GetEnvelopeFilterType();
	int envelopeOscillatorChoice = (int)_configuration->GetEnvelopeFilterOscillatorType();

	auto oscillatorStrs = std::vector<std::string>({
		"Sine",
		"Square",
		"Triangle",
		"Sawtooth",
		"Random"
	});
	auto envelopeFilterStrs = std::vector<std::string>({
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
		ftxui::Toggle(envelopeFilterStrs, &envelopeFilterEnabled) | ftxui::align_right,
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

	//auto envelopeTypeUI = ftxui::Renderer([&] {

	//	// Manual 
	//	if (envelopeFilterTypeChoice == 0)
	//		return ftxui::text("Manual Settings");

	//	// Oscillator
	//	else if (envelopeFilterTypeChoice == 1)
	//	{
	//		return envelopeOscillatorUI->Render();
	//	}

	//	// Sweep
	//	else if (envelopeFilterTypeChoice == 2)
	//	{
	//		return envelopeSweepUI->Render();
	//	}

	//	else
	//		throw new std::exception("Unhandled Envelope Filter Type Choice");
	//});

	auto envelopeTypeUI = ftxui::Container::Vertical({
		ftxui::Renderer([&] { return ftxui::text("Manual Settings"); }) | ftxui::Maybe([&] { return envelopeFilterTypeChoice == 0; }),
		envelopeOscillatorUI | ftxui::Maybe([&]{ return envelopeFilterTypeChoice == 1; }),
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
		ftxui::Slider(&filterResonanceStr, &filterResonance, filterResonanceMin, filterResonanceMax, filterResonanceIncr),
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

	auto synthSettings = ftxui::Container::Horizontal({

		oscillatorUIRenderer | ftxui::flex_grow | ftxui::border,
		envelopeUIRenderer | ftxui::flex_grow | ftxui::border,
		filterUIRenderer | ftxui::flex_grow | ftxui::border

	}) | ftxui::flex_grow;

	// UI BACKEND LOOP!! This will be run just for re-drawing purposes during our
	//					 primary loop below.
	//
	auto renderer = ftxui::Renderer(synthSettings, [&]
	{
		// Synth Information
		auto synthInformation = ftxui::vbox(
		{
			ftxui::text("Current Time (s): " + std::to_string(systemTime / 1000.0)),
			ftxui::text("Stream Latency (ms): " + std::to_string(streamLatency)),
			ftxui::text("Sample Rate (Hz): " + std::to_string(1000.0 / averageAudioTime)),

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
		//_lock->lock();

		lastUIUpdate += audioStopWatch.markMilliseconds();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			screen.ExitLoopClosure();
			break;
		}

		// CRITICAL SECTION: This section is for updating the piano notes. So, this should be a fast loop; and 
		//					 able to run before the playback thread has to wait too long 
		// 
		//					 (less than a sample, ideally)
		//
		_lock->lock();

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
			if (((SynthPlaybackDevice*)_player->GetDevice())->HasNote(midiNote) && !isPressed)
				((SynthPlaybackDevice*)_player->GetDevice())->SetNote(midiNote, false, _player->GetStreamTime());

			// Engage
			else if (!((SynthPlaybackDevice*)_player->GetDevice())->HasNote(midiNote) && isPressed)
				((SynthPlaybackDevice*)_player->GetDevice())->SetNote(midiNote, true, _player->GetStreamTime());
		}

		// Clean Up Synth Notes
		((SynthPlaybackDevice*)_player->GetDevice())->ClearUnused(_player->GetStreamTime());

		_lock->unlock();

		// Get Current Time
		//systemTime += rtAudio->isStreamOpen() ? rtAudio->getStreamTime() : audioStopWatch.markMilliseconds();

		// Get Stream Latency
		//long latency = rtAudio->isStreamOpen() ? rtAudio->getStreamLatency() : 0;

		//streamLatency = (latency / (double)SAMPLING_RATE) * 2;

		// Forces a redraw
		//screen.PostEvent(ftxui::Event::Custom);

		// Run UI Backend
		//if (lastUIUpdate > 100)
		//	loop.RunOnce();

		//Envelope noteEnvelope(noteAttack, noteDecay, noteSustain, noteRelease, envelope.GetAttackPeak(), envelope.GetSustainPeak());

		//if (noteEnvelope != envelope)
		//	_configuration->SetNoteEnvelope(noteEnvelope);

		// Synth Configuration:  Our copy is updated on the UI Update timer
		//
		if (lastUIUpdate > 100)
		{
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
		}

		// Only update if changes were made (~100ms)
		if (_configuration->IsDirty())
		{
			// CRITICAL SECTION:  This is an update from the UI, which will reset the synth parameters. So,
			//					  it is only allowed every ~100ms at the most.
			//
			_lock->lock();

			//_player->StopStream();

			((SynthPlaybackDevice*)_player->GetDevice())->UpdateSynth(*_configuration);

			//_player->StartStream();

			// Reset Configuration Flag
			_configuration->ClearDirty();

			// Reset UI Update Time
			lastUIUpdate = 0;

			_lock->unlock();
		}
			

		loop.RunOnce();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		//_lock->unlock();
	}

	// This may be required for some UI loop functions that did / did not exit properly
	//screen.Exit();
}


int main(int argc, char* argv[], char* envp[])
{
	_lock = new std::mutex();

	// Read midi file
	if (argc > 1)
	{
	}

	// Manual keyboard input
	else
	{
		Initialize();
	}

	LoopUI();

	if (_player->IsStreamRunning())
		_player->StopStream();

	if (_player->IsStreamOpen())
		_player->CloseStream();

	// Delete memory for primary components
	//
	//if (_player != NULL)
	//	delete _player;

	return 0;
}