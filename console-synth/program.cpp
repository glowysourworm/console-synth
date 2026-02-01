
#include "AudioPlayer.h"
#include "RtAudio.h"
#include "StopWatch.h"
#include "SynthPlayer.h"
#include <Windows.h>
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>

#include <string>

// Static Instance (RT Audio) (Couldn't figure out how to cast function callbacks to the framework)
AudioPlayer* _player;

int PrimaryAudioCallback(void* outputBuffer,
	void* inputBuffer,
	unsigned int nFrames,
	double streamTime,
	RtAudioStreamStatus status,
	void* userData)
{
	return _player->GetDevice()->RtAudioCallback(outputBuffer, inputBuffer, nFrames, streamTime, status, userData);
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
	auto renderer = ftxui::Renderer([&systemTime, &streamLatency, &averageAudioTime](bool focused)
		{
			// Create a simple document with three text elements.
			return ftxui::vbox({
			  ftxui::text("Current Time (s): " + std::to_string(systemTime / 1000.0)),
			  ftxui::text("Stream Latency (ms): " + std::to_string(streamLatency)),
			  ftxui::text("Sample Rate (Hz): " + std::to_string(1000.0 / averageAudioTime)) }) | ftxui::border;
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

		// Get Current Time
		//systemTime += rtAudio->isStreamOpen() ? rtAudio->getStreamTime() : audioStopWatch.markMilliseconds();

		// Get Stream Latency
		//long latency = rtAudio->isStreamOpen() ? rtAudio->getStreamLatency() : 0;

		//streamLatency = (latency / (double)SAMPLING_RATE) * 2;

		// Forces a redraw
		screen.PostEvent(ftxui::Event::Custom);

		// Run UI Backend
		loop.RunOnce();

		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}


int main(int argc, char* argv[], char* envp[])
{
	// Initialize audio device
	AudioPlayer* player = NULL;

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
	if (player != NULL)
		delete player;

	return 0;
}