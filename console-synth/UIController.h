#pragma once
#include "BaseController.h"
#include "CompressorUI.h"
#include "DelayUI.h"
#include "EnvelopeFilterUI.h"
#include "EnvelopeUI.h"
#include "OscillatorUI.h"
#include "OutputUI.h"
#include "PlaybackParameters.h"
#include "ReverbUI.h"
#include "SynthConfiguration.h"
#include "SynthInformationUI.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/loop.hpp>
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
	/// Returns true if there is a dirty status on the UI. This means that data needs to be
	/// collected during this pass.
	/// </summary>
	bool IsDirty() const;

	/// <summary>
	/// Takes data from the UI and updates the configuration
	/// </summary>
	void FromUI(SynthConfiguration* configuration);

	/// <summary>
	/// Pushes data from the playback parameters to the UI
	/// </summary>
	void ToUI(const PlaybackParameters* parameters);

private:

	void ThreadStart();

private:

	ftxui::Component _view;							// Shared Pointer (std::shared_pointer)

	SynthInformationUI* _synthInformationUI;
	CompressorUI* _compressorUI;
	DelayUI* _delayUI;
	EnvelopeUI* _envelopeUI;
	EnvelopeFilterUI* _envelopeFilterUI;
	OscillatorUI* _oscillatorUI;
	OutputUI* _outputUI;
	ReverbUI* _reverbUI;

	// This was needed to accomodate the FTXUI stack-based API (ftxui::Screen does not have a way to create a private pointer)
	std::thread* _thread;
	std::mutex* _lock;
};

