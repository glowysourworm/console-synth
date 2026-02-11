#pragma once
#include "AudioController.h"
#include "LoopTimer.h"
#include "SynthConfiguration.h"
#include "UIController.h"
#include <thread>

class MainController
{
public:

	MainController();
	~MainController();

	/// <summary>
	/// Initialization function for the synth backend. This must be called before starting the player!
	/// </summary>
	bool Initialize();

	/// <summary>
	/// Disposes of backend, and controller resources
	/// </summary>
	bool Dispose();

	/// <summary>
	/// Runs until the application should exit (this could be put on a separate thread.. if it helps CPU load.. 
	/// MSFT may have some loading difference with a background thread)
	/// </summary>
	void Loop();

private:

	AudioController* _audioController;
	UIController* _uiController;

	SynthConfiguration* _configuration;

	LoopTimer* _uiTimer;
};