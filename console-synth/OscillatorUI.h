#pragma once

#include "Constant.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

class OscillatorUI : public UIBase
{
public:
	OscillatorUI(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor);
	~OscillatorUI();

	ftxui::Element Render() override;
	ftxui::Component GetRenderer() override;
	ftxui::Component GetComponent() override;

	AmplitudeOscillatorType GetSelection() const;

private:

	// Oscillator Selected Index
	int* _oscillatorSelectedIndex;

	// Oscillator Choices
	std::vector<std::string>* _oscillatorItems;
};

