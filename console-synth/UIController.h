#pragma once
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
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>

class UIController
{
public:

	UIController(const SynthConfiguration* configuration);
	~UIController();

	void Initialize(const PlaybackParameters* parameters);
	void Dispose();

public:

	/// <summary>
	/// Run one cycle of the UI; and return true if the UI can continue running, or needs
	/// to exit
	/// </summary>
	bool RunOnce() const;

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

	const SynthConfiguration* _configuration;

	ftxui::ScreenInteractive* _screen;
	ftxui::Loop* _loop;

	SynthInformationUI* _synthInformationUI;
	CompressorUI* _compressorUI;
	DelayUI* _delayUI;
	EnvelopeUI* _envelopeUI;
	EnvelopeFilterUI* _envelopeFilterUI;
	OscillatorUI* _oscillatorUI;
	OutputUI* _outputUI;
	ReverbUI* _reverbUI;
};

