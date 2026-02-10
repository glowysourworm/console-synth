#pragma once

#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

class ReverbUI : UIBase
{
public:
	ReverbUI(bool enabled, float delay, float gain, const std::string& title, const ftxui::Color& titleColor);
	~ReverbUI();

	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	bool GetEnabled() const;

	float GetDelay() const;
	float GetGain() const;
	
private:

	std::vector<std::string>* _onOffStrs;

	SliderUI* _delayUI;
	SliderUI* _gainUI;

	// Set to zero to choose "On"
	int _enabledIndex;
};

