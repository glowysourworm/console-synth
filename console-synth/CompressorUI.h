#pragma once

#include "SliderUI.h"
#include "SynthConfiguration.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <memory>
#include <string>
#include <vector>

class CompressorUI : public UIBase
{
public:
	CompressorUI(bool enabled, 
				 float threshold, 
				 float gain, 
				 float attack, 
				 float release, 
				 float compressionRatio, 
				 const std::string& title, 
				 const ftxui::Color& titleColor);
	~CompressorUI();

	void Initialize(const SynthConfiguration* configuration) override;
	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	bool GetEnabled() const;

	float GetThreshold() const;
	float GetGain() const;
	float GetAttack() const;
	float GetRelease() const;
	float GetCompressionRatio() const;
private:

	std::vector<std::string>* _onOffStrs;

	int _enabledIndex;

	SliderUI* _thresholdUI;
	SliderUI* _gainUI;
	SliderUI* _attackUI;
	SliderUI* _releaseUI;
	SliderUI* _compressionRatioUI;
};

