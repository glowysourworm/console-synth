#include "CompressorUI.h"
#include "Envelope.h"
#include "EnvelopeUI.h"
#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

CompressorUI::CompressorUI(bool enabled, 
	float threshold, 
	float gain, 
	float attack, 
	float release, 
	float compressionRatio, 
	const std::string& title, 
	const ftxui::Color& titleColor) : UIBase(title, true, titleColor)
{
	_thresholdUI = new SliderUI(threshold, 0.01f, 1.0f, 0.01f, "Threshold {:.2f}", titleColor);
	_gainUI = new SliderUI(gain, 0.01f, 1.0f, 0.01f, "Gain  {:.2f}", titleColor);
	_attackUI = new SliderUI(attack, 0.005f, 0.5f, 0.005f, "Attack (s) {:.3f}", titleColor);
	_releaseUI = new SliderUI(release, 0.005f, 0.5f, 0.005f, "Release {:.2f}", titleColor);
	_compressionRatioUI = new SliderUI(compressionRatio, 1.0f, 10.0f, 0.1f, "Compression Ratio {:.1f}", titleColor);
	
	_onOffStrs = new std::vector<std::string>({ "On", "Off" });
}

CompressorUI::~CompressorUI()
{
	delete _thresholdUI;
	delete _gainUI;
	delete _attackUI;
	delete _releaseUI;
	delete _compressionRatioUI;

	delete _onOffStrs;
}

ftxui::Element CompressorUI::Render()
{
	return this->GetComponent()->Render();
}

ftxui::Component CompressorUI::GetRenderer()
{
	// Compressor UI
	auto componentUI = this->GetComponent();

	return ftxui::Renderer(componentUI, [&] { return componentUI->Render(); });
}

ftxui::Component CompressorUI::GetComponent()
{
	// Compressor (Enable)
	auto enableUI = ftxui::Container::Horizontal({

		ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }) | ftxui::flex_grow,
		ftxui::Toggle(_onOffStrs, &_enabledIndex) | ftxui::align_right,
	});

	// Compressor
	return ftxui::Container::Vertical(
	{
		enableUI,
		ftxui::Renderer([&] { return ftxui::separator(); }),

		_thresholdUI->GetComponent(),
		_gainUI->GetComponent(),
		_attackUI->GetComponent(),
		_releaseUI->GetComponent(),
		_compressionRatioUI->GetComponent()
	});
}

bool CompressorUI::GetEnabled() const
{
	return _enabledIndex == 0;
}

float CompressorUI::GetThreshold() const
{
	return _thresholdUI->GetValue();
}

float CompressorUI::GetGain() const
{
	return _gainUI->GetValue();
}

float CompressorUI::GetAttack() const
{
	return _attackUI->GetValue();
}

float CompressorUI::GetRelease() const
{
	return _releaseUI->GetValue();
}

float CompressorUI::GetCompressionRatio() const
{
	return _compressionRatioUI->GetValue();
}
