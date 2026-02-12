#include "CompressorUI.h"
#include "DelayUI.h"
#include "EnvelopeFilterUI.h"
#include "EnvelopeUI.h"
#include "OscillatorUI.h"
#include "OutputUI.h"
#include "PlaybackParameters.h"
#include "ReverbUI.h"
#include "SynthConfiguration.h"
#include "SynthInformationUI.h"
#include "UIController.h"
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <memory>
#include <mutex>
#include <thread>

UIController::UIController()
{
	_thread = nullptr;
	_lock = nullptr;
	_oscillatorUI = nullptr;
	_envelopeUI = nullptr;
	_envelopeFilterUI = nullptr;
	_compressorUI = nullptr;
	_outputUI = nullptr;
	_reverbUI = nullptr;
	_delayUI = nullptr;

}

UIController::~UIController()
{
	delete _oscillatorUI;
	delete _envelopeUI;
	delete _envelopeFilterUI;
	delete _compressorUI;
	delete _outputUI;
	delete _reverbUI;
	delete _delayUI;
}

bool UIController::Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters)
{
	// FTX-UI (Terminal Loop / Renderer)
	// 
	// Their backend will handle interaction, resizing, and redrawing. The document should be
	// updated on this thread (AFAIK) which is captured in the lambda function(s). 
	// 
	// After more working with FTXUI, it is using std::move, and std::make_shared, and std::shared_ptr
	// to try to keep stack allocations, and move them to the heap. I'm not as familiar with this pattern;
	// but it is very difficult to reproduce an inheritance pattern. Though, not impossible, it would
	// be a better use of time to just leave it on our stack, which will accomplish the same task. 
	// 
	// There is a slight memory leak which may be in FTXUI; but I'm not sure, yet, if RT Audio has issues,
	// or just our SynthNote* allocations (probably not).
	//
	// https://arthursonzogni.github.io/FTXUI/
	//

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

	auto synthInputSettings = ftxui::Container::Horizontal({

		_oscillatorUI->GetComponent() | ftxui::flex | ftxui::border,
		_envelopeUI->GetComponent() | ftxui::flex | ftxui::border,
		_envelopeFilterUI->GetComponent() | ftxui::flex | ftxui::border

	}) | ftxui::flex_grow;

	auto synthEffectsSettings = ftxui::Container::Horizontal({

		_compressorUI->GetComponent() | ftxui::flex_grow | ftxui::border,
		_reverbUI->GetComponent() | ftxui::flex_grow | ftxui::border,
		_delayUI->GetComponent() | ftxui::flex_grow | ftxui::border

	});

	auto synthOutputSettings = ftxui::Container::Horizontal({

		_outputUI->GetComponent() | ftxui::flex_grow | ftxui::border,

	});

	// UI BACKEND LOOP!! This will be run just for re-drawing purposes during our
	//					 primary loop below.
	//
	_view = ftxui::Container::Vertical({
		_synthInformationUI->GetComponent() | ftxui::flex_grow,		 
		synthInputSettings | ftxui::flex_grow,
		synthEffectsSettings | ftxui::flex_grow,
		synthOutputSettings | ftxui::flex_grow,
	}) | ftxui::CatchEvent([&](ftxui::Event event) {

		// Only allow mouse events through
		if (event.mouse().button == ftxui::Mouse::Left)
		{
			//isUIDirty |= event.is_mouse();
			return false;
		}

		// Cancel keyboard events
		return true;

	}) | ftxui::flex_grow;

	_thread = new std::thread(&UIController::ThreadStart, this);
	_lock = new std::mutex();

	return true;
}

bool UIController::Dispose()
{
	_thread->join();
	delete _thread;
	delete _lock;
	_thread = nullptr;
	_lock = nullptr;

	return true;
}

bool UIController::IsDirty() const
{
	return _oscillatorUI->GetDirty() ||
			_envelopeUI->GetDirty() ||
			_envelopeFilterUI->GetDirty() ||
			_delayUI->GetDirty() ||
			_reverbUI->GetDirty() ||
			_compressorUI->GetDirty() ||
			_outputUI->GetDirty();

	return false;
}

void UIController::FromUI(SynthConfiguration* configuration)
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
}

void UIController::ToUI(const PlaybackParameters* parameters)
{
	_lock->lock();

	// Synth Information
	_synthInformationUI->Update(parameters);

	// TODO
	_outputUI->SetOutput(0, 0);

	_lock->unlock();
}

void UIController::ThreadStart()
{
	auto screen = ftxui::ScreenInteractive::Fullscreen();
	auto loop = ftxui::Loop(&screen, _view);

	// FTXUI has an option to create an event loop (this will run their backend UI code)
	//
	// https://arthursonzogni.com/FTXUI/doc/examples_2component_2custom_loop_8cpp-example.html#_a8
	//

	while (!loop.HasQuitted())
	{
		// These were added to help create UI classes. The stack-oriented rendering
		// architecture of FTXUI is tricky to get to provide an update each call. You
		// basically have to either follow their UI inheritance pattern (closely), or
		// you have to add something to trigger re-rendering!
		_oscillatorUI->UpdateComponent(true);
		_envelopeUI->UpdateComponent(true);
		_envelopeFilterUI->UpdateComponent(true);

		_delayUI->UpdateComponent(true);
		_reverbUI->UpdateComponent(true);
		_compressorUI->UpdateComponent(true);

		_outputUI->UpdateComponent(true);

		// Use custom event to force one UI update
		screen.PostEvent(ftxui::Event::Custom);
		loop.RunOnce();

		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}
