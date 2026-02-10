#pragma once

#include "Envelope.h"
#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

class EnvelopeUI : public UIBase
{
public:
	EnvelopeUI(const Envelope& envelope, const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor);
	~EnvelopeUI();

	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	Envelope GetSelection() const;

private:

	// Oscillator Selected Index
	Envelope* _envelope;

	// Slider UI's
	SliderUI* _attack;
	SliderUI* _decay;
	SliderUI* _release;
	SliderUI* _attackPeak;
	SliderUI* _sustainPeak;
};

