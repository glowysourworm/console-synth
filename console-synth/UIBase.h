#pragma once

#include "SynthConfiguration.h"
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
	/// Initializes UI component from synth configuration
	/// </summary>
	/// <param name="configuration">Configuration (if needed for the UI component) is provided</param>
	virtual void Initialize(const SynthConfiguration* configuration);

	/// <summary>
	/// Creates FTXUI dom element for rendering
	/// </summary>
	ftxui::Element Render();

	/// <summary>
	/// Creaets FTXUI component renderer for use in rendering the underlying UI
	/// </summary>
	ftxui::Component GetRenderer();

	/// <summary>
	/// Gets FTXUI component for rendering
	/// </summary>
	virtual ftxui::Component GetComponent() = 0;

	/// <summary>
	/// Forces component to update itself, recreating any variables that it needs for
	/// the drawing code to render it.
	/// </summary>
	virtual void UpdateComponent(bool clearDirty) = 0;

	/// <summary>
	/// Returns status of the UI (has there been changes)
	/// </summary>
	virtual bool GetDirty() const;

protected:

	void SetDirty();
	void ClearDirty();

protected:

	std::string* title;
	ftxui::Color* titleColor;
	bool showBoxedWithTitle;

	ftxui::Component _componentUI;

private:

	bool _isDirty;
	bool _isInitialized;
};

