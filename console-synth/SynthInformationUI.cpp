#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include "SynthInformationUI.h"
#include "UIBase.h"
#include <format>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

SynthInformationUI::SynthInformationUI(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor)
	: UIBase(title, showBoxedWithTitle, titleColor)
{
	_hostApi = new std::string("");
	_deviceName = new std::string("");
	_streamFormat = new std::string("");
	_streamBufferSize = new std::string("");
	_samplingRate = new std::string("");

	_averageUITime = new std::string("");
	_averageCallbackTime = new std::string("");
	_averageFrontendTime = new std::string("");
	_streamTime = new std::string("");
	_streamLatency = new std::string("");
}
SynthInformationUI::~SynthInformationUI()
{
	delete _hostApi;
	delete _deviceName;
	delete _streamFormat;
	delete _streamBufferSize;
	delete _samplingRate;

	delete _averageUITime;
	delete _averageCallbackTime;
	delete _averageFrontendTime;
	delete _streamTime;
	delete _streamLatency;
}
void SynthInformationUI::Initialize(const SynthConfiguration* configuration)
{
	UIBase::Initialize(configuration);
}
ftxui::Component SynthInformationUI::GetComponent()
{
	// Synth Information
	auto synthInformation = ftxui::Container::Horizontal({

		ftxui::Renderer([&] {
			return ftxui::vbox(
			{
				ftxui::text("Host API:                 " + *_hostApi),
				ftxui::text("Device Name:              " + *_deviceName),
				ftxui::text("Stream Format:            " + *_streamFormat),
				ftxui::text("Stream Buffer Size:       " + *_streamBufferSize),
				ftxui::text("Sample Rate (Hz):         " + *_samplingRate)

			}) | ftxui::flex_grow;
		}),

		ftxui::Renderer([&] { return ftxui::separator(); }),

		ftxui::Renderer([&] {
			return ftxui::vbox(
			{
				ftxui::text(" Stream Time            (s):    " + *_streamTime),
				ftxui::text(" Avg. UI Time          (ms):    " + *_averageUITime),
				ftxui::text(" Avg. Callback Time    (ms):    " + *_averageCallbackTime),
				ftxui::text(" Avg. Frontend Time    (ms):    " + *_averageFrontendTime),
				ftxui::text(" Stream Latency   (samples):    " + *_streamLatency)

			}) | ftxui::flex_grow;
		})
	});

	return ftxui::Container::Vertical({

		// Synth Title
		ftxui::Renderer([&] 
		{ 
			return ftxui::vbox({
				ftxui::text(*this->title) | ftxui::color(*this->titleColor),
				ftxui::separator(),
			});
		}),

		// Synth Information
		synthInformation,

		// Footer
		ftxui::Renderer([&]
		{
			return ftxui::vbox({
				ftxui::separator()
			});
		})
	}) | ftxui::border;
}
void SynthInformationUI::UpdateComponent(bool clearDirty)
{
	if (clearDirty)
		this->ClearDirty();
}
void SynthInformationUI::Update(const PlaybackParameters* parameters)
{
	_hostApi->clear();
	_hostApi->append(parameters->GetHostApi());

	_deviceName->clear();
	_deviceName->append(parameters->GetDeviceName());

	_streamFormat->clear();
	_streamFormat->append(parameters->GetDeviceFormat());

	_streamBufferSize->clear();
	_streamBufferSize->append(std::format("{} (frames)", parameters->GetOutputBufferFrameSize()));

	_samplingRate->clear();
	_samplingRate->append(std::to_string(parameters->GetSamplingRate()));
	


	_streamTime->clear();
	_streamTime->append(std::format("{:.3f}", parameters->GetStreamTime()));

	_averageUITime->clear();
	_averageUITime->append(std::format("{:.3f}", parameters->GetAvgUIMilli()));
	
	_averageCallbackTime->clear();
	_averageCallbackTime->append(std::format("{:.3f}", parameters->GetAvgAudioMilli()));

	_averageFrontendTime->clear();
	_averageFrontendTime->append(std::format("{:.3f}", parameters->GetAvgFrontendMilli()));
	
	_streamLatency->clear();
	_streamLatency->append(std::to_string(parameters->GetStreamLatency()));

	// Signal Dirty
	this->SetDirty();
}