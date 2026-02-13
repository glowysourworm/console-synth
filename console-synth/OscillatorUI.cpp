#include "Constant.h"
#include "OscillatorUI.h"
#include "SynthConfiguration.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
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

void OscillatorUI::Initialize(const SynthConfiguration* configuration)
{
	UIBase::Initialize(configuration);
}

ftxui::Component OscillatorUI::GetComponent()
{
	auto radioboxUI = ftxui::Radiobox(_oscillatorItems, _oscillatorSelectedIndex);

	if (!this->showBoxedWithTitle)
	{
		return radioboxUI | ftxui::CatchEvent([&](ftxui::Event event) {

			// Dirty Indicator
			this->SetDirty();

			return true;
		});
	}
	else
	{
		auto componentUI = ftxui::Container::Vertical(
		{
			ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }),
			ftxui::Renderer([&] { return ftxui::separator(); }),
			radioboxUI
		});

		return componentUI;
	}
}

void OscillatorUI::UpdateComponent(bool clearDirty)
{
	if (clearDirty)
		this->ClearDirty();
}

OscillatorType OscillatorUI::GetSelection() const
{
	return (OscillatorType)*_oscillatorSelectedIndex;
}
