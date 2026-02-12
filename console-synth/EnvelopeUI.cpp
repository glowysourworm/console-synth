#include "Envelope.h"
#include "EnvelopeUI.h"
#include "SliderUI.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

EnvelopeUI::EnvelopeUI(const Envelope& envelope, const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor) : UIBase(title, showBoxedWithTitle, titleColor)
{
	_envelope = new Envelope(envelope);

	_attack = new SliderUI(envelope.GetAttack(), 0.01f, 1.0f, 0.01f,           "Attack   (s) {:.2f}", ftxui::Color::White);
	_decay = new SliderUI(envelope.GetDecay(), 0.01f, 2.0f, 0.01f,             "Decay    (s) {:.2f}", ftxui::Color::White);
	_release = new SliderUI(envelope.GetRelease(), 0.01f, 3.0f, 0.01f,         "Release  (s) {:.2f}", ftxui::Color::White);
	_attackPeak = new SliderUI(envelope.GetAttackPeak(), 0.01f, 1.0f, 0.01f,   "Limit        {:.2f}", ftxui::Color::White);
	_sustainPeak = new SliderUI(envelope.GetSustainPeak(), 0.01f, 1.0f, 0.01f, "Sustain      {:.2f}", ftxui::Color::White);
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

ftxui::Component EnvelopeUI::GetComponent()
{
	if (this->showBoxedWithTitle)
	{
		auto componentUI = ftxui::Container::Vertical(
		{
			ftxui::Renderer([&] { return ftxui::text(*this->title) | ftxui::color(*this->titleColor); }),
			ftxui::Renderer([&] { return ftxui::separator(); }),

			_attack->GetRenderer(),
			_decay->GetComponent(),
			_release->GetComponent(),

			ftxui::Renderer([&] { return ftxui::separator(); }),

			_attackPeak->GetComponent(),
			_sustainPeak->GetComponent()
		}) | ftxui::flex_grow;

		return componentUI;
	}
	else
	{
		auto componentUI = ftxui::Container::Vertical(
		{
			_attack->GetComponent(),
			_decay->GetComponent(),
			_release->GetComponent(),

			ftxui::Renderer([&] { return ftxui::separator(); }),

			_attackPeak->GetComponent(),
			_sustainPeak->GetComponent()
		}) | ftxui::flex_grow;

		return componentUI;
	}
}

void EnvelopeUI::UpdateComponent(bool clearDirty)
{
	// Limit Sustain Peak to Attack Peak
	if (_sustainPeak->GetValue() > _attackPeak->GetValue())
		_sustainPeak->SetValue(_attackPeak->GetValue());

	_attack->UpdateComponent(clearDirty);
	_decay->UpdateComponent(clearDirty);
	_release->UpdateComponent(clearDirty);
	_attackPeak->UpdateComponent(clearDirty);
	_sustainPeak->UpdateComponent(clearDirty);

	// Envelope <- UI
	_envelope->Set(_attack->GetValue(), _decay->GetValue(), 0, _release->GetValue(), _attackPeak->GetValue(), _sustainPeak->GetValue());

	if (clearDirty)
		this->ClearDirty();
}

Envelope EnvelopeUI::GetSelection() const
{
	return *_envelope;
}

bool EnvelopeUI::GetDirty() const
{
	return _attack->GetDirty() ||
		   _decay->GetDirty() ||
		   _release->GetDirty() ||
		   _attackPeak->GetDirty() ||
		   _sustainPeak->GetDirty();
}
