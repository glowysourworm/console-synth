#include "OutputUI.h"
#include "SliderUI.h"
#include "SynthConfiguration.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

OutputUI::OutputUI(float gain, float leftRight, const std::string& title, const ftxui::Color& titleColor) : UIBase(title, true, titleColor)
{
	_gainUI = new SliderUI(gain, 0.00f, 1.0f, 0.01f,          "Gain:       {:.2f}", titleColor);
	_leftRightUI = new SliderUI(leftRight, 0.0f, 1.0f, 0.01f, "L/R:        {:.2f}", titleColor);

    _left = 0;
    _right = 0;
}

OutputUI::~OutputUI()
{
	delete _gainUI;
	delete _leftRightUI;
}

void OutputUI::Initialize(const SynthConfiguration* configuration)
{
    UIBase::Initialize(configuration);

    _gainUI->Initialize(configuration);
    _leftRightUI->Initialize(configuration);
}

ftxui::Component OutputUI::GetComponent()
{
    // Output
    auto componentUI = ftxui::Container::Vertical(
    {
        ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }),
        ftxui::Renderer([&] {return ftxui::separator(); }),

        _gainUI->GetRenderer(),
        _leftRightUI->GetRenderer(),

        ftxui::Renderer([&] {
            return ftxui::vbox({
                ftxui::separator(),
                ftxui::hbox({
                    ftxui::text("Left ") | ftxui::center,
                    ftxui::border(ftxui::gauge(_left) | ftxui::color(*this->titleColor))
                }),
                ftxui::hbox({
                    ftxui::text("Right") | ftxui::center,
                    ftxui::border(ftxui::gauge(_right) | ftxui::color(*this->titleColor))
                })
            });
        })
    });

    return componentUI;
}

void OutputUI::UpdateComponent(bool clearDirty)
{
    _gainUI->UpdateComponent(clearDirty);
    _leftRightUI->UpdateComponent(clearDirty);

    if (clearDirty)
        this->ClearDirty();
}

float OutputUI::GetGain() const
{
	return _gainUI->GetValue();
}

float OutputUI::GetLeftRight() const
{
	return _leftRightUI->GetValue();
}

void OutputUI::SetOutput(float left, float right)
{
    _left = left;
    _right = right;
}

bool OutputUI::GetDirty() const
{
    return _gainUI->GetDirty() || _leftRightUI->GetDirty();
}
