#include "Envelope.h"
#include "EnvelopeUI.h"
#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

EnvelopeUI::EnvelopeUI(const Envelope& envelope, const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor) : UIBase(title, showBoxedWithTitle, titleColor)
{
	_envelope = new Envelope(envelope);

	_attack = new SliderUI(envelope.GetAttack(), 0.01f, 1.0f, 0.01f, "Attack  (s) {:.2f}", ftxui::Color::White);
	_decay = new SliderUI(envelope.GetDecay(), 0.01f, 2.0f, 0.01f, "Decay  (s) {:.2f}", ftxui::Color::White);
	_release = new SliderUI(envelope.GetRelease(), 0.01f, 3.0f, 0.01f, "Release  (s) {:.2f}", ftxui::Color::White);
	_attackPeak = new SliderUI(envelope.GetAttackPeak(), 0.01f, 1.0f, 0.01f, "Limit {:.2f}", ftxui::Color::White);
	_sustainPeak = new SliderUI(envelope.GetSustainPeak(), 0.01f, 1.0f, 0.01f, "Sustain {:.2f}", ftxui::Color::White);
}

EnvelopeUI::~EnvelopeUI()
{
	delete _envelope;
	delete _attack;
	delete _decay;
	delete _release;
	delete _attackPeak;
	delete _sustainPeak;
}

ftxui::Element EnvelopeUI::Render()
{
	return this->GetComponent()->Render();
}

ftxui::Component EnvelopeUI::GetRenderer()
{
	auto envelopeUI = this->GetComponent();

	// Passing in the component lets FTXUI manage focus for the component (ONLY!)
	return ftxui::Renderer(envelopeUI, [&] { return envelopeUI->Render(); });
}

ftxui::Component EnvelopeUI::GetComponent()
{
	if (this->showBoxedWithTitle)
	{
		return ftxui::Container::Vertical(
		{
			ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }),
			ftxui::Renderer([&] { return ftxui::separator(); }),

			_attack->GetComponent(),
			_decay->GetComponent(),
			_release->GetComponent(),

			ftxui::Renderer([&] { return ftxui::separator(); }),

			_attackPeak->GetComponent(),
			_sustainPeak->GetComponent()
		});
	}
	else
	{
		return ftxui::Container::Vertical(
		{
			_attack->GetComponent(),
			_decay->GetComponent(),
			_release->GetComponent(),

			ftxui::Renderer([&] { return ftxui::separator(); }),

			_attackPeak->GetComponent(),
			_sustainPeak->GetComponent()
		});
	}
}

Envelope EnvelopeUI::GetSelection() const
{
	return *_envelope;
}
