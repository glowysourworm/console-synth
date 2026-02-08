#include "Constant.h"
#include "OscillatorUI.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>


OscillatorUI::OscillatorUI(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor) : UIBase(title, showBoxedWithTitle, titleColor)
{
	_oscillatorItems = new std::vector<std::string>({
		"Sine",
		"Square",
		"Triangle",
		"Sawtooth",
		"Random"
	});
	_oscillatorSelectedIndex = new int(0);
}
OscillatorUI::~OscillatorUI()
{
	UIBase::~UIBase();

	delete _oscillatorItems;
	delete _oscillatorSelectedIndex;
}

ftxui::Element OscillatorUI::Render()
{
	return this->GetComponent()->Render();
}

ftxui::Component OscillatorUI::GetRenderer()
{
	auto componentUI = this->GetComponent();

	return ftxui::Renderer(componentUI, [&]
	{ 
		return componentUI->Render();
	});
}

ftxui::Component OscillatorUI::GetComponent()
{
	auto radioboxUI = ftxui::Radiobox(_oscillatorItems, _oscillatorSelectedIndex);

	if (!this->showBoxedWithTitle)
	{
		return radioboxUI;
	}
	else
	{
		return ftxui::Container::Vertical(
		{
			ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }),
			ftxui::Renderer([&] { return ftxui::separator(); }),
			radioboxUI
		});
	}
}

AmplitudeOscillatorType OscillatorUI::GetSelection() const
{
	return (AmplitudeOscillatorType)*_oscillatorSelectedIndex;
}
