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
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

class MainUI : public UIBase
{
public:

	MainUI(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor);
	~MainUI();

	void Initialize(const SynthConfiguration* configuration) override;
	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	/// <summary>
	/// Collects information from the UI using a prepared configuration (pre-locked!)
	/// </summary>
	void FromUI(SynthConfiguration* configuration);

	/// <summary>
	/// Sets the UI from the update parameters (prepared for use by this component)
	/// </summary>
	void ToUI(const PlaybackParameters* parameters);

	bool GetDirty() const override;

private:

	std::vector<std::string>* _tabHeaders;

	ftxui::Component _mainControl;
	ftxui::Component _tabControl;							// Shared Pointer (std::shared_pointer)
	ftxui::Component _tabControlMenu;
	int* _tabIndex;

	SynthInformationUI* _synthInformationUI;
	CompressorUI* _compressorUI;
	DelayUI* _delayUI;
	EnvelopeUI* _envelopeUI;
	EnvelopeFilterUI* _envelopeFilterUI;
	OscillatorUI* _oscillatorUI;
	OutputUI* _outputUI;
	ReverbUI* _reverbUI;

};

