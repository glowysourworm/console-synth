#pragma once

#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

class DelayUI : UIBase
{
public:
	DelayUI(bool enabled, bool feedbackEnabled, float delay, float gain, float wetDry, const std::string& title, const ftxui::Color& titleColor);
	~DelayUI();

	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	bool GetEnabled() const;
	bool GetFeedbackEnabled() const;

	float GetDelay() const;
	float GetGain() const;
	float GetWetDry() const;

private:

	std::vector<std::string>* _onOffStrs;

	SliderUI* _delayUI;
	SliderUI* _gainUI;
	SliderUI* _wetDryUI;

	// Set to zero to choose "On"
	int _enabledIndex;
	int _feedbackIndex;
};

