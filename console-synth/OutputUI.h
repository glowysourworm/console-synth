#pragma once

#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

class OutputUI : public UIBase
{
public:
	OutputUI(float gain, float leftRight, const std::string& title, const ftxui::Color& titleColor);
	~OutputUI();

	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	float GetGain() const;
	float GetLeftRight() const;

	void SetOutput(float left, float right);

private:

	SliderUI* _gainUI;
	SliderUI* _leftRightUI;

	float _left;
	float _right;
};

