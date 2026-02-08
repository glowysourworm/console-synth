#include "DelayUI.h"
#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

DelayUI::DelayUI(bool enabled, bool feedbackEnabled, float delay, float gain, const std::string& title, const ftxui::Color& titleColor) : UIBase(title, true, titleColor)
{
    _enabledIndex = enabled ? 0 : 1;
    _feedbackIndex = feedbackEnabled ? 0 : 1;
    _delayUI = new SliderUI(delay, 0.005f, 0.5f, 0.005f, "Delay  (s) {:.3f}", titleColor);
    _gainUI = new SliderUI(gain, 0.01f, 1.0f, 0.01f, "Gain {:.2f}", titleColor);
    _onOffStrs = new std::vector<std::string>({ "On", "Off" });
}

DelayUI::~DelayUI()
{
    delete _delayUI;
    delete _gainUI;
    delete _onOffStrs;
}

ftxui::Element DelayUI::Render()
{
    return this->GetComponent()->Render();
}

ftxui::Component DelayUI::GetRenderer()
{
    // Delay UI
    auto componentUI = this->GetComponent();

    return ftxui::Renderer(componentUI, [&] { return componentUI->Render(); });
}

ftxui::Component DelayUI::GetComponent()
{
    // Delay (Enable)
    auto enableUI = ftxui::Container::Horizontal({

        ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }) | ftxui::flex_grow,
        ftxui::Toggle(_onOffStrs, &_enabledIndex) | ftxui::align_right,
    });

    // Delay Feedback (Enable)
    auto feedbackEnableUI = ftxui::Container::Horizontal({

        ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }) | ftxui::flex_grow,
        ftxui::Toggle(_onOffStrs, &_feedbackIndex) | ftxui::align_right,
    });

    // Delay
    return ftxui::Container::Vertical(
    {
        enableUI,
        ftxui::Renderer([&] {return ftxui::separator(); }),

        feedbackEnableUI,
        ftxui::Renderer([&] {return ftxui::separator(); }),

        _delayUI->GetComponent(),
        _gainUI->GetComponent()
    });
}

bool DelayUI::GetEnabled() const
{
    return _enabledIndex == 0;
}

bool DelayUI::GetFeedbackEnabled() const
{
    return _feedbackIndex == 0;
}

float DelayUI::GetDelay() const
{
    return _delayUI->GetValue();
}

float DelayUI::GetGain() const
{
    return _delayUI->GetValue();
}
