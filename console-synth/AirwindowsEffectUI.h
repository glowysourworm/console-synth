#pragma once

#include "SliderUI.h"
#include "SynthConfiguration.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

class AirwindowsEffectUI : public UIBase
{
public:

	/// <summary>
	/// Creates an AirwindowsEffectUI using the name (key) from the airwin registry. During initialization, the 
	/// UI will load the appropriate effects sliders for the parameters.
	/// </summary>
	/// <param name="airwinPluginName">Airwin registry key (name). This should be from the SynthConfiguration*</param>
	AirwindowsEffectUI(const std::string& airwinPluginName, bool showBoxedWithTitle, const ftxui::Color& titleColor);
	~AirwindowsEffectUI();

	void Initialize(const SynthConfiguration* configuration) override;
	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	bool GetDirty() const override;

private:

	std::vector<SliderUI*>* _parameterUIs;
};

