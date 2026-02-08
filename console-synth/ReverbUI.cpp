#include "ReverbUI.h"
#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

ReverbUI::ReverbUI(bool enabled, float delay, float gain, const std::string& title, const ftxui::Color& titleColor) : UIBase(title, true, titleColor)
{
    _enabledIndex = enabled ? 0 : 1;
    _delayUI = new SliderUI(delay, 0.005f, 0.5f, 0.005f, "Delay  (s) {:.3f}", titleColor);
    _gainUI = new SliderUI(gain, 0.01f, 1.0f, 0.01f, "Gain {:.2f}", titleColor);
    _onOffStrs = new std::vector<std::string>({ "On", "Off" });
}

ReverbUI::~ReverbUI()
{
    delete _delayUI;
    delete _gainUI;
    delete _onOffStrs;
}

ftxui::Element ReverbUI::Render()
{
    return this->GetComponent()->Render();
}

ftxui::Component ReverbUI::GetRenderer()
{
    // Reverb UI
    auto componentUI = this->GetComponent();

    return ftxui::Renderer(componentUI, [&] { return componentUI->Render(); });
}

ftxui::Component ReverbUI::GetComponent()
{
    // Reverb (Enable)
    auto reverbLabelUI = ftxui::Container::Horizontal({

        ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }) | ftxui::flex_grow,
        ftxui::Toggle(_onOffStrs, &_enabledIndex) | ftxui::align_right,
    });

    // Reverb
    return ftxui::Container::Vertical(
    {
        reverbLabelUI,
        ftxui::Renderer([&] {return ftxui::separator(); }),

        _delayUI->GetComponent(),
        _gainUI->GetComponent()
    });
}

bool ReverbUI::GetEnabled() const
{
    return _enabledIndex == 0;
}

float ReverbUI::GetDelay() const
{
    return _delayUI->GetValue();
}

float ReverbUI::GetGain() const
{
    return _delayUI->GetValue();
}
