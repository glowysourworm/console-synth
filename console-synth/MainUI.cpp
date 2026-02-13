#include "CompressorUI.h"
#include "DelayUI.h"
#include "EnvelopeFilterUI.h"
#include "EnvelopeUI.h"
#include "MainUI.h"
#include "OscillatorUI.h"
#include "OutputUI.h"
#include "PlaybackParameters.h"
#include "ReverbUI.h"
#include "SynthConfiguration.h"
#include "SynthInformationUI.h"
#include "UIBase.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <vector>

MainUI::MainUI(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor) : UIBase(title, showBoxedWithTitle, titleColor)
{
	// Tab Headers
	_tabHeaders = new std::vector<std::string>({
		"Output",
		"Synth",
		"Midi"
		});

	// Selected Tab
	_tabIndex = new int(0);

	_synthInformationUI = nullptr;
	_oscillatorUI = nullptr;
	_envelopeUI = nullptr;
	_envelopeFilterUI = nullptr;
	_reverbUI = nullptr;
	_delayUI = nullptr;
	_compressorUI = nullptr;
	_outputUI = nullptr;
}

MainUI::~MainUI()
{
	delete _synthInformationUI;
	delete _oscillatorUI;
	delete _envelopeUI;
	delete _envelopeFilterUI;
	delete _reverbUI;
	delete _delayUI;
	delete _compressorUI;
	delete _outputUI;
}

void MainUI::Initialize(const SynthConfiguration* configuration)
{
	UIBase::Initialize(configuration);

	// Synth Information
	_synthInformationUI = new SynthInformationUI("Terminal Synth", true, ftxui::Color::GreenYellow);

	// Source Oscillator
	_oscillatorUI = new OscillatorUI("Oscillator", true, ftxui::Color::Blue);

	// Note Envelope
	_envelopeUI = new EnvelopeUI(configuration->GetNoteEnvelope(), "Input Envelope", true, ftxui::Color::BlueLight);

	// Envelope Filter (Oscillator)
	_envelopeFilterUI = new EnvelopeFilterUI(configuration->GetEnvelopeFilterType(),
		configuration->GetEnvelopeFilterOscillatorType(),
		configuration->GetEnvelopeFilter(),
		configuration->GetEnvelopeFilterCutoff(),
		configuration->GetEnvelopeFilterResonance(),
		configuration->GetEnvelopeFilterOscillatorFrequency(),
		configuration->GetHasEnvelopeFilter(),
		"Envelope Filter", ftxui::Color::Red);
	// Reverb
	_reverbUI = new ReverbUI(configuration->GetHasReverb(),
		configuration->GetReverbDelaySeconds(),
		configuration->GetReverbGain(),
		configuration->GetReverbWetDry(),
		"Reverb", ftxui::Color::Green);

	// Delay
	_delayUI = new DelayUI(configuration->GetHasDelay(),
		configuration->GetDelayFeedback(),
		configuration->GetDelaySeconds(),
		configuration->GetDelayGain(),
		configuration->GetDelayWetDry(),
		"Delay", ftxui::Color::Purple);

	// Compressor
	_compressorUI = new CompressorUI(configuration->GetHasCompressor(),
		configuration->GetCompressorThreshold(),
		configuration->GetCompressorGain(),
		configuration->GetCompressorAttack(),
		configuration->GetCompressorRelease(),
		configuration->GetCompressionRatio(),
		"Compressor", ftxui::Color::HotPink);

	// Output
	_outputUI = new OutputUI(configuration->GetOutputGain(), configuration->GetOutputLeftRight(), "Output", ftxui::Color::Green);

	_synthInformationUI->Initialize(configuration);
	_oscillatorUI->Initialize(configuration);
	_envelopeUI->Initialize(configuration);
	_envelopeFilterUI->Initialize(configuration);
	_reverbUI->Initialize(configuration);
	_delayUI->Initialize(configuration);
	_compressorUI->Initialize(configuration);
	_outputUI->Initialize(configuration);

	auto midiSettings = ftxui::Container::Horizontal({

	});

	auto synthSignalSettings = ftxui::Container::Horizontal({

		_oscillatorUI->GetComponent() | ftxui::flex | ftxui::border,
		_envelopeUI->GetComponent() | ftxui::flex | ftxui::border,
		_envelopeFilterUI->GetComponent() | ftxui::flex | ftxui::border

	}) | ftxui::flex_grow;

	auto synthEffectsSettings = ftxui::Container::Horizontal({

		_compressorUI->GetComponent() | ftxui::flex_grow | ftxui::border,
		_reverbUI->GetComponent() | ftxui::flex_grow | ftxui::border,
		_delayUI->GetComponent() | ftxui::flex_grow | ftxui::border

	});

	// Synth Tab
	auto synthSettings = ftxui::Container::Vertical({
		synthSignalSettings,
		synthEffectsSettings
	});

	// Output Tab
	auto synthOutput = ftxui::Container::Vertical({

		_synthInformationUI->GetComponent() | ftxui::flex_grow,
		_outputUI->GetComponent() | ftxui::flex_grow ,

	});

	// Primary UI
	_tabControlMenu = ftxui::Menu(_tabHeaders, _tabIndex, ftxui::MenuOption::HorizontalAnimated());
	
	_tabControl = ftxui::Container::Tab({
		synthOutput | ftxui::flex_grow,
		synthSettings | ftxui::flex_grow,
		midiSettings | ftxui::flex_grow,
	}, _tabIndex) | ftxui::CatchEvent([&](ftxui::Event event) {

	// Only allow mouse events through
	if (event.mouse().button == ftxui::Mouse::Left)
	{
		//isUIDirty |= event.is_mouse();
		return false;
	}

	// Cancel keyboard events
	return true;

	}) | ftxui::flex_grow;

	_mainControl = ftxui::Container::Vertical({
		_tabControlMenu,
		_tabControl
	});
}

ftxui::Component MainUI::GetComponent()
{
	return _mainControl;
}

void MainUI::UpdateComponent(bool clearDirty)
{
}

void MainUI::FromUI(SynthConfiguration* configuration)
{
	// Enable
	configuration->SetHasEnvelopeFilter(_envelopeFilterUI->GetEnabled());
	configuration->SetHasDelay(_delayUI->GetEnabled());
	configuration->SetHasCompressor(_compressorUI->GetEnabled());
	configuration->SetHasReverb(_reverbUI->GetEnabled());

	// Oscillator
	configuration->SetOscillatorType(_oscillatorUI->GetSelection());

	// Envelope
	configuration->SetNoteEnvelope(_envelopeUI->GetSelection());

	// Envelope Filter
	configuration->SetEnvelopeFilter(_envelopeFilterUI->GetEnvelope());
	configuration->SetEnvelopeFilterType(_envelopeFilterUI->GetType());
	configuration->SetEnvelopeFilterCutoff(_envelopeFilterUI->GetCutoff());
	configuration->SetEnvelopeFilterResonance(_envelopeFilterUI->GetResonance());
	configuration->SetEnvelopeFilterOscillatorFrequency(_envelopeFilterUI->GetOscillatorFrequency());
	configuration->SetEnvelopeFilterOscillatorType(_envelopeFilterUI->GetOscillatorType());

	// Reverb
	configuration->SetReverbSeconds(_reverbUI->GetDelay());
	configuration->SetReverbGain(_reverbUI->GetGain());
	configuration->SetReverbWetDry(_reverbUI->GetWetDry());

	// Delay
	configuration->SetDelayFeedback(_delayUI->GetFeedbackEnabled());
	configuration->SetDelaySeconds(_delayUI->GetDelay());
	configuration->SetDelayGain(_delayUI->GetGain());
	configuration->SetDelayWetDry(_delayUI->GetWetDry());

	// Compressor
	configuration->SetCompressionRatio(_compressorUI->GetCompressionRatio());
	configuration->SetCompressorAttack(_compressorUI->GetAttack());
	configuration->SetCompressorGain(_compressorUI->GetGain());
	configuration->SetCompressorRelease(_compressorUI->GetRelease());
	configuration->SetCompressorThreshold(_compressorUI->GetThreshold());

	// Output
	configuration->SetOutputGain(_outputUI->GetGain());
	configuration->SetOutputLeftRight(_outputUI->GetLeftRight());

	// Clear Dirty UI Status
	_oscillatorUI->UpdateComponent(true);
	_envelopeUI->UpdateComponent(true);
	_envelopeFilterUI->UpdateComponent(true);
	_delayUI->UpdateComponent(true);
	_reverbUI->UpdateComponent(true);
	_compressorUI->UpdateComponent(true);
	_outputUI->UpdateComponent(true);
}

void MainUI::ToUI(const PlaybackParameters* parameters)
{
	// Synth Information
	_synthInformationUI->Update(parameters);

	// Synth Output Channels
	_outputUI->SetOutput(parameters->GetOutputLeft(), parameters->GetOutputRight());
}

bool MainUI::GetDirty() const
{
	return _oscillatorUI->GetDirty() ||
		_envelopeUI->GetDirty() ||
		_envelopeFilterUI->GetDirty() ||
		_delayUI->GetDirty() ||
		_reverbUI->GetDirty() ||
		_compressorUI->GetDirty() ||
		_outputUI->GetDirty();
}
