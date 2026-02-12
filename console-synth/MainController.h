#pragma once
#include "AudioController.h"
#include "BaseController.h"
#include "LoopTimer.h"
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include "UIController.h"

class MainController : public BaseController
{
public:

	MainController();
	~MainController();

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) override;
	bool Dispose() override;

	/// <summary>
	/// Runs until the application should exit (this could be put on a separate thread.. if it helps CPU load.. 
	/// MSFT may have some loading difference with a background thread)
	/// </summary>
	void Loop();

private:

	// Primary Owner of SynthConfiguration*
	SynthConfiguration* _configuration;

	AudioController* _audioController;
	UIController* _uiController;

	LoopTimer* _uiTimer;
};