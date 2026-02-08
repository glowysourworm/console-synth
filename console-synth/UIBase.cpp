#include "UIBase.h"
#include <ftxui/screen/color.hpp>
#include <string>

UIBase::UIBase(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor)
{
	this->title = new std::string(title);
	this->titleColor = new ftxui::Color(titleColor);
	this->showBoxedWithTitle = showBoxedWithTitle;
}

UIBase::~UIBase()
{
	delete this->title;
}
