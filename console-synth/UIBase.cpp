#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

UIBase::UIBase(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor)
{
	this->title = new std::string(title);
	this->titleColor = new ftxui::Color(titleColor);
	this->showBoxedWithTitle = showBoxedWithTitle;

	_componentUI = nullptr;
	_isDirty = false;
}

UIBase::~UIBase()
{
	delete this->title;
	delete this->titleColor;
}

ftxui::Element UIBase::Render()
{
	this->UpdateComponent(false);

	if (_componentUI == nullptr)
		_componentUI = this->GetComponent();

	return _componentUI->Render();
}

ftxui::Component UIBase::GetRenderer()
{
	if (_componentUI == nullptr)
		_componentUI = this->GetComponent();

	return ftxui::Renderer(_componentUI, [&] {

		return _componentUI->Render();
	});
}

bool UIBase::GetDirty() const
{
	return _isDirty;
}

void UIBase::SetDirty()
{
	_isDirty = true;
}

void UIBase::ClearDirty()
{
	_isDirty = false;
}
