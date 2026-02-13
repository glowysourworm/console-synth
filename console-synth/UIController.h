#pragma once
#include "BaseController.h"
#include "MainUI.h"
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include <mutex>
#include <thread>

class UIController : public BaseController
{
public:

	UIController();
	~UIController();

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) override;
	bool Dispose() override;

public:

	/// <summary>
	/// (Shared Main Thread / UI Thread) Returns true if there is a dirty status on the UI. This means that data needs to be
	/// collected during this pass.
	/// </summary>
	bool IsDirty() const;

	/// <summary>
	/// (Shared Main Thread / UI Thread) Takes data from the UI and updates the configuration
	/// </summary>
	void FromUI(SynthConfiguration* configuration);

	/// <summary>
	/// (Shared Main Thread / UI Thread) Pushes data from the playback parameters to the UI
	/// </summary>
	void ToUI(const PlaybackParameters* parameters);

private:

	void ThreadStart();

private:

	MainUI* _mainUI;

	// This was needed to accomodate the FTXUI stack-based API (ftxui::Screen does not have a way to create a private pointer)
	std::thread* _thread;
	std::mutex* _lock;
};

