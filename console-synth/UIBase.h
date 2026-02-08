#pragma once

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

/// <summary>
/// Class container for ftxui components. Provides functions for both Element
/// and Component rendering situations.
/// </summary>
class UIBase
{
public:

	UIBase(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor);
	~UIBase();

	/// <summary>
	/// Creates FTXUI dom element for rendering
	/// </summary>
	virtual ftxui::Element Render() = 0;

	/// <summary>
	/// Creaets FTXUI component renderer for use in rendering the underlying UI
	/// </summary>
	virtual ftxui::Component GetRenderer() = 0;

	/// <summary>
	/// Gets FTXUI component for rendering
	/// </summary>
	virtual ftxui::Component GetComponent() = 0;

protected:

	std::string* title;
	ftxui::Color* titleColor;
	bool showBoxedWithTitle;
};

