#pragma once

#include "Constant.h"
#include "Envelope.h"
#include "EnvelopeUI.h"
#include "OscillatorUI.h"
#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

class EnvelopeFilterUI : UIBase
{
public:
	EnvelopeFilterUI(EnvelopeFilterType type, 
					 AmplitudeOscillatorType oscillatorType, 
					 const Envelope& envelope, 
					 float cutoff, 
					 float resonance, 
					 float oscillatorFrequency,
					 bool enabled,
					 const std::string& title, 
					 const ftxui::Color& titleColor);
	~EnvelopeFilterUI();

	ftxui::Element Render() override;
	ftxui::Component GetRenderer() override;
	ftxui::Component GetComponent() override;

	EnvelopeFilterType GetType() const;
	AmplitudeOscillatorType GetOscillatorType() const;
	Envelope GetEnvelope() const;
	float GetOscillatorFrequency() const;
	float GetCutoff() const;
	float GetResonance() const;

	bool GetEnabled() const;


private:

	std::vector<std::string>* _onOffStrs;
	std::vector<std::string>* _filterTypeStrs;

	OscillatorUI* _oscillatorUI;
	EnvelopeUI* _envelopeUI;
	SliderUI* _cutoffUI;
	SliderUI* _resonanceUI;
	SliderUI* _oscillatorFrequencyUI;

	// Set to zero to choose "On"
	int _enabledIndex;
	int _filterTypeIndex;
	int _oscillatorTypeIndex;
};

