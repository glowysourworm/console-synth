#include "SliderUI.h"
#include "UIBase.h"
#include <format>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
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
    _label = new std::string();
}

SliderUI::~SliderUI()
{
    delete _value;
    delete _minValue;
    delete _maxValue;
    delete _increment;
    delete _labelFormat;
    delete _label;
}

ftxui::Component SliderUI::GetComponent()
{
    this->UpdateComponent(false);

    // Create Component
    auto componentUI = ftxui::Slider(_label, _value, _minValue, _maxValue, _increment) | ftxui::color(*this->titleColor);

    return componentUI;
}

void SliderUI::UpdateComponent(bool clearDirty)
{
    // Update Label
    auto labelFormat = std::string_view(_labelFormat->c_str());

    _label->erase(0, _label->size());
    _label->append(std::vformat(labelFormat, std::make_format_args(*_value)));

    if (clearDirty)
        this->ClearDirty();
}

float SliderUI::GetValue() const
{
    return *_value;
}

void SliderUI::SetValue(float newValue)
{
    (*_value) = newValue;
}
