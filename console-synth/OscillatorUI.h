#pragma once

#include "Constant.h"
#include "SynthConfiguration.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

class OscillatorUI : public UIBase
{
public:
	OscillatorUI(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor);
	~OscillatorUI();

	void Initialize(const SynthConfiguration* configuration) override;
	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	OscillatorType GetSelection() const;

private:

	// Oscillator Selected Index
	int* _oscillatorSelectedIndex;

	// Oscillator Choices
	std::vector<std::string>* _oscillatorItems;
};

