#include "Constant.h"
#include "Envelope.h"
#include "EnvelopeFilterUI.h"
#include "EnvelopeUI.h"
#include "OscillatorUI.h"
#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

EnvelopeFilterUI::EnvelopeFilterUI(EnvelopeFilterType type, 
								   OscillatorType oscillatorType,
								   const Envelope& envelope, 
								   float cutoff, 
								   float resonance, 
								   float oscillatorFrequency,
								   bool enabled,
								   const std::string& title, 
								   const ftxui::Color& titleColor) : UIBase(title, true, titleColor)
{
	_filterTypeIndex = (int)type;
	_oscillatorTypeIndex = (int)oscillatorType;
	_enabledIndex = enabled ? 0 : 1;
	_cutoffUI = new SliderUI(cutoff, 20, 7000, 10,            "Cutoff (Hz)       {:.0f}", titleColor);
	_resonanceUI = new SliderUI(resonance, 0.01f, 1.0f, 0.01f,         "Resonance:        {:.2f}", titleColor);
	_oscillatorFrequencyUI = new SliderUI(oscillatorFrequency, 0.2f, 3.0f, 0.1f, "Osc. Freq.  (Hz): {:.2f}", titleColor);

	_oscillatorUI = new OscillatorUI("Oscillator", true, titleColor);
	_envelopeUI = new EnvelopeUI(envelope, "Filter Envelope", true, titleColor);

	_onOffStrs = new std::vector<std::string>({"On", "Off"});
	_filterTypeStrs = new std::vector<std::string>({ "Manual", "Oscillator", "Sweep" });
}

EnvelopeFilterUI::~EnvelopeFilterUI()
{
	delete _cutoffUI;
	delete _resonanceUI;
	delete _oscillatorFrequencyUI;
	delete _envelopeUI;
	delete _oscillatorUI;
	delete _onOffStrs;
	delete _filterTypeStrs;
}

ftxui::Component EnvelopeFilterUI::GetComponent()
{
	// Envelope Filter Label (Enable)
	auto envelopeFilterLabelUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }) | ftxui::flex_grow,
		ftxui::Toggle(_onOffStrs, &_enabledIndex) | ftxui::align_right,
	});

	// Envelope Filter Type Label (Toggle)
	auto envelopeFilterTypeLabelUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text("Filter Type") | ftxui::color(*this->titleColor); }) | ftxui::flex_grow,
		ftxui::Toggle(_filterTypeStrs, &_filterTypeIndex) | ftxui::align_right,
	});

	// Envelope Type Chooser
	auto envelopeTypeUI = ftxui::Container::Vertical({

		// Manual 
		ftxui::Renderer([&] { return ftxui::text("Manual Settings"); }) | ftxui::Maybe([&] { return _filterTypeIndex == 0; }),

		// Oscillator
		_oscillatorUI->GetRenderer() | ftxui::Maybe([&] { return _filterTypeIndex == 1; }),

		// Sweep
		_envelopeUI->GetRenderer() | ftxui::Maybe([&] { return _filterTypeIndex == 2; })
	});

	// Envelope Filter
	auto componentUI = ftxui::Container::Vertical(
	{
		envelopeFilterLabelUI,
		ftxui::Renderer([&] {return ftxui::separator(); }),

		envelopeFilterTypeLabelUI,
		ftxui::Renderer([&] {return ftxui::separator(); }),

		envelopeTypeUI,
		ftxui::Renderer([&] {return ftxui::separator(); }),

		_cutoffUI->GetRenderer(),
		_resonanceUI->GetRenderer()
	});

	return componentUI;
}

void EnvelopeFilterUI::UpdateComponent(bool clearDirty)
{
	_oscillatorUI->UpdateComponent(clearDirty);
	_oscillatorFrequencyUI->UpdateComponent(clearDirty);
	_envelopeUI->UpdateComponent(clearDirty);
	_cutoffUI->UpdateComponent(clearDirty);
	_resonanceUI->UpdateComponent(clearDirty);

	if (clearDirty)
		this->ClearDirty();
}

EnvelopeFilterType EnvelopeFilterUI::GetType() const
{
	return (EnvelopeFilterType)_filterTypeIndex;
}

OscillatorType EnvelopeFilterUI::GetOscillatorType() const
{
	return (OscillatorType)_oscillatorTypeIndex;
}

Envelope EnvelopeFilterUI::GetEnvelope() const
{
	return _envelopeUI->GetSelection();
}

float EnvelopeFilterUI::GetOscillatorFrequency() const
{
	return _oscillatorFrequencyUI->GetValue();
}

float EnvelopeFilterUI::GetCutoff() const
{
	return _cutoffUI->GetValue();
}

float EnvelopeFilterUI::GetResonance() const
{
	return _resonanceUI->GetValue();
}

bool EnvelopeFilterUI::GetEnabled() const
{
	return _enabledIndex == 0;
}
