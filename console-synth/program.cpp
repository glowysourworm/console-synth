
#include "AudioPlayer.h"
#include "RtAudio.h"
#include "StopWatch.h"
#include "SynthPlaybackDevice.h"
#include "SynthPlayer.h"
#include "WindowsKeyCodes.h"
#include <Windows.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <mutex>
#include <string>

// Static Instance (RT Audio) (Couldn't figure out how to cast function callbacks to the framework)
SynthPlayer* _player;
std::mutex* _lock;

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

void LoopUI()
{
	StopWatch uiStopWatch, audioStopWatch;
	bool exit = false;

	// Initialize Stop Watches
	audioStopWatch.mark();
	uiStopWatch.mark();

	// Get Current Time
	double systemTime = 0;
	double streamLatency = 0;
	double averageAudioTime = 0;

	bool midi60 = false;
	bool midi61 = false;
	bool midi62 = false;
	bool midi63 = false;
	bool midi64 = false;
	bool midi65 = false;
	bool midi66 = false;
	bool midi67 = false;
	bool midi68 = false;
	bool midi69 = false;
	bool midi70 = false;

	// FTX-UI (Terminal Loop / Renderer)
	// 
	// Their backend will handle interaction, resizing, and redrawing. The document should be
	// updated on this thread (AFAIK) which is captured in the lambda function.
	//
	// https://arthursonzogni.github.io/FTXUI/
	//

	auto screen = ftxui::ScreenInteractive::Fullscreen();

	// UI BACKEND LOOP!! This will be run just for re-drawing purposes during our
	//					 primary loop below.
	//
	auto renderer = ftxui::Renderer([&systemTime,
		&streamLatency,
		&averageAudioTime,
		&midi60,
		&midi61,
		&midi62,
		&midi63,
		&midi64,
		&midi65,
		&midi66,
		&midi67,
		&midi68,
		&midi69,
		&midi70](bool focused)
	{
		std::string notes = "";

		if (midi60)
			notes += "60";

		if (midi61)
			notes += "61";

		if (midi62)
			notes += "62";

		if (midi63)
			notes += "63";

		if (midi64)
			notes += "64";

		if (midi65)
			notes += "65";

		if (midi66)
			notes += "66";

		if (midi67)
			notes += "67";

		if (midi68)
			notes += "68";

		if (midi69)
			notes += "69";

		if (midi70)
			notes += "70";

		// Create a simple document with three text elements.
		return ftxui::vbox({
		  ftxui::text("Current Time (s): " + std::to_string(systemTime / 1000.0)),
		  ftxui::text("Stream Latency (ms): " + std::to_string(streamLatency)),
		  ftxui::text("Sample Rate (Hz): " + std::to_string(1000.0 / averageAudioTime)),
		  ftxui::text("Notes Engaged (MIDI): " + notes)
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
	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
			break;



		midi60 = GetAsyncKeyState(WindowsKeyCodes::A) != 0;
		midi61 = GetAsyncKeyState(WindowsKeyCodes::W) != 0;
		midi62 = GetAsyncKeyState(WindowsKeyCodes::S) != 0;
		midi63 = GetAsyncKeyState(WindowsKeyCodes::E) != 0;
		midi64 = GetAsyncKeyState(WindowsKeyCodes::D) != 0;
		midi65 = GetAsyncKeyState(WindowsKeyCodes::J) != 0;
		midi66 = GetAsyncKeyState(WindowsKeyCodes::I) != 0;
		midi67 = GetAsyncKeyState(WindowsKeyCodes::K) != 0;
		midi68 = GetAsyncKeyState(WindowsKeyCodes::O) != 0;
		midi69 = GetAsyncKeyState(WindowsKeyCodes::L) != 0;
		midi70 = GetAsyncKeyState(VK_OEM_1) != 0;

		// Lock for RT Audio thread
		_lock->lock();

		// Go ahead and process all possible MIDI keys
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(60, midi60, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(61, midi61, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(62, midi62, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(63, midi63, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(64, midi64, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(65, midi65, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(66, midi66, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(67, midi67, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(68, midi68, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(69, midi69, _player->GetStreamTime());
		((SynthPlaybackDevice*)_player->GetDevice())->SetNote(70, midi70, _player->GetStreamTime());

		_lock->unlock();

		// Get Current Time
		//systemTime += rtAudio->isStreamOpen() ? rtAudio->getStreamTime() : audioStopWatch.markMilliseconds();

		// Get Stream Latency
		//long latency = rtAudio->isStreamOpen() ? rtAudio->getStreamLatency() : 0;

		//streamLatency = (latency / (double)SAMPLING_RATE) * 2;

		// Forces a redraw
		screen.PostEvent(ftxui::Event::Custom);

		// Run UI Backend
		loop.RunOnceBlocking();

		//std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	// This may be required for some UI loop functions that did / did not exit properly
	screen.Exit();
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
		_player = new SynthPlayer();

		_player->Initialize(&PrimaryAudioCallback, &PrimaryErrorCallback);
	}

	LoopUI();

	if (_player->IsStreamRunning())
		_player->StopStream();

	if (_player->IsStreamOpen())
		_player->CloseStream();

	// Delete memory for primary components
	//
	if (_player != NULL)
		delete _player;

	return 0;
}