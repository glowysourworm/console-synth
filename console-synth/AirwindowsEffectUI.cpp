#include "AirwindowsEffectUI.h"
#include "SliderUI.h"
#include "SynthConfiguration.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

AirwindowsEffectUI::AirwindowsEffectUI(const std::string& airwindowsPluginName, bool showBoxedWithTitle, const ftxui::Color& titleColor)
	: UIBase(airwindowsPluginName, showBoxedWithTitle, titleColor)
{
	_parameterUIs = new std::vector<SliderUI*>();
}

AirwindowsEffectUI::~AirwindowsEffectUI()
{
	for (int index = 0; index < _parameterUIs->size(); index++)
	{
		delete _parameterUIs->at(index);
	}

	delete _parameterUIs;
}

void AirwindowsEffectUI::Initialize(const SynthConfiguration* configuration)
{
	// Airwindows Plugin Details
	auto plugin = configuration->GetEffectRegistry()->GetPlugin(*this->title);

	// Parameters
	for (int index = 0; index < plugin->GetNumberOfParams(); index++)
	{
		char label[100];
		char value[100];
		char units[100];
		plugin->GetEffect()->getParameterName(index, label);			// Name of parameter
		plugin->GetEffect()->getParameterDisplay(index, value);			// Current value of parameter ("display of it")
		plugin->GetEffect()->getParameterLabel(index, units);			// Units for parameter ("Label underneath")

		std::string labelFormat = std::string(label) + " (" + std::string(units) + "):  {:.2f}";
		float initialValue = plugin->GetEffect()->getParameter(index);

		SliderUI* sliderUI = new SliderUI(initialValue, 0.0f, 1.0f, 0.01f, labelFormat, *this->titleColor);

		_parameterUIs->push_back(sliderUI);
	}	
}

ftxui::Component AirwindowsEffectUI::GetComponent()
{
	std::vector<ftxui::Component> components;

	// We have to get components from our UI container for the vector - which will re-render each time.
	for (int index = 0; index < _parameterUIs->size(); index++)
	{
		components.push_back(_parameterUIs->at(index)->GetComponent());
	}

	return ftxui::Container::Vertical(components);
}

void AirwindowsEffectUI::UpdateComponent(bool clearDirty)
{
	for (int index = 0; index < _parameterUIs->size(); index++)
	{
		_parameterUIs->at(index)->UpdateComponent(clearDirty);
	}
}

bool AirwindowsEffectUI::GetDirty() const
{
	bool isDirty = false;

	for (int index = 0; index < _parameterUIs->size() && !isDirty; index++)
	{
		isDirty |= _parameterUIs->at(index)->GetDirty();
	}

	return isDirty;
}
