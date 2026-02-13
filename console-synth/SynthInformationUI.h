#pragma once
#include "PlaybackParameters.h"
#include "SynthConfiguration.h"
#include "UIBase.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

class SynthInformationUI : public UIBase
{
public:
	SynthInformationUI(const std::string& title, bool showBoxedWithTitle, const ftxui::Color& titleColor);
	~SynthInformationUI();

	void Initialize(const SynthConfiguration* configuration) override;
	ftxui::Component GetComponent() override;
	void UpdateComponent(bool clearDirty) override;

	void Update(const PlaybackParameters* parameters);

private:

	std::string* _hostApi;
	std::string* _deviceName;
	std::string* _streamFormat;
	std::string* _streamBufferSize;
	std::string* _samplingRate;

	std::string* _averageUITime;
	std::string* _averageCallbackTime;
	std::string* _averageFrontendTime;
	std::string* _streamTime;
	std::string* _streamLatency;
};


