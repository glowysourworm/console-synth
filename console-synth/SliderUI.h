#pragma once

#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

class SliderUI : public UIBase
{
public:

	SliderUI(float initialValue,
			 float minValue,
			 float maxValue,
			 float increment,
			 const std::string& labelFormat,
			 const ftxui::Color& titleColor);
	~SliderUI();

	ftxui::Element Render() override;
	ftxui::Component GetRenderer() override;
	ftxui::Component GetComponent() override;

	float GetValue() const;

private:

	float* _value;
	float* _minValue;
	float* _maxValue;
	float* _increment;

	std::string* _labelFormat;
};

