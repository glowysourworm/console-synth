#pragma once
#include <string>
struct PlaybackParameters
{
	std::string* hostAPI;
	unsigned int samplingRate;
	unsigned int numberOfChannels;
	unsigned int outputBufferFrameSize;
};

