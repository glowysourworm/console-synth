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
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <mutex>
#include <string>
#include <thread>


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

	float envelopeMin = 0.01f;
	float envelopeMax = 1.0f;
	float envelopeIncrement = 0.01f;

	float noteAttack = envelope.GetAttack();
	float noteSustain = envelope.GetSustain();
	float noteDecay = envelope.GetDecay();
	float noteRelease = envelope.GetRelease();

	auto envelopeUI = ftxui::Container::Vertical(
	{
		ftxui::Slider("Attack  (s) " + std::to_string(noteAttack), &noteAttack, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider("Sustain (s) " + std::to_string(noteSustain), &noteSustain, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider("Decay   (s) " + std::to_string(noteDecay), &noteDecay, envelopeMin, envelopeMax, envelopeIncrement),
		ftxui::Slider("Release (s) " + std::to_string(noteRelease), &noteRelease, envelopeMin, envelopeMax, envelopeIncrement),
	});

	// UI BACKEND LOOP!! This will be run just for re-drawing purposes during our
	//					 primary loop below.
	//
	auto renderer = ftxui::Renderer(envelopeUI, [&]
	{
		// Text Elements
		return ftxui::vbox(
			{
				ftxui::text("Current Time (s): " + std::to_string(systemTime / 1000.0)),
				ftxui::text("Stream Latency (ms): " + std::to_string(streamLatency)),
				ftxui::text("Sample Rate (Hz): " + std::to_string(1000.0 / averageAudioTime)),

				// Inputs
				envelopeUI->Render(),

			}) | ftxui::border;

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

		// Only update if changes were made
		//if (_configuration->IsDirty())
		//{
		//	//_lock->lock();

		//	_player->StopStream();

		//	((SynthPlaybackDevice*)_player->GetDevice())->UpdateSynth(*_configuration);

		//	_player->StartStream();

		//	//_lock->unlock();

		//	// Reset Configuration Flag
		//	_configuration->ClearDirty();
		//}
			

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