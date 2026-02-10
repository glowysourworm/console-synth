#pragma once

#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
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

	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	float GetValue() const;
	void SetValue(float newValue);

private:

	float* _value;
	float* _minValue;
	float* _maxValue;
	float* _increment;

	std::string* _label;
	std::string* _labelFormat;
};

