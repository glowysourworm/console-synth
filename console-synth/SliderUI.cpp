#include "SliderUI.h"
#include "UIBase.h"
#include <exception>
#include <format>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <string_view>

SliderUI::SliderUI(float initialValue, 
                   float minValue, 
                   float maxValue, 
                   float increment, 
                   const std::string& labelFormat, 
                   const ftxui::Color& titleColor) : UIBase(labelFormat, false, titleColor)
{
    _value = new float(initialValue);
    _minValue = new float(minValue);
    _maxValue = new float(maxValue);
    _increment = new float(increment);
    _labelFormat = new std::string(labelFormat);
}

SliderUI::~SliderUI()
{
    delete _value;
    delete _minValue;
    delete _maxValue;
    delete _increment;
    delete _labelFormat;
}

ftxui::Element SliderUI::Render()
{
    // Create Component
    auto sliderUI = this->GetComponent();

    return sliderUI->Render();
}

ftxui::Component SliderUI::GetRenderer()
{
    if (this->showBoxedWithTitle)
    {
        throw new std::exception("Not implemented");
    }
    else
    {
        auto sliderUI = this->GetComponent();

        return ftxui::Renderer(sliderUI, [&] { return this->Render(); });
    }
}

ftxui::Component SliderUI::GetComponent()
{
    // Update Label
    auto labelFormat = std::string_view(_labelFormat->c_str());
    std::string label = std::vformat(labelFormat, std::make_format_args(*_value));

    // Create Component
    return ftxui::Slider(&label, _value, _minValue, _maxValue, _increment) | ftxui::color(*this->titleColor);
}

float SliderUI::GetValue() const
{
    return *_value;
}
