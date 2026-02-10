#pragma once
#include <string>

class PlaybackParameters
{
public:

	PlaybackParameters(
		const char* hostApi, 
		const char* deviceFormat, 
		const char* deviceName, 
		unsigned int samplingRate, 
		unsigned int numberOfChannels, 
		unsigned int outputBufferFrameSize)
	{
		_hostApi = new std::string(hostApi);
		_deviceFormat = new std::string();
		_deviceName = new std::string(deviceName);
		_samplingRate = samplingRate;
		_numberOfChannels = numberOfChannels;
		_outputBufferFrameSize = outputBufferFrameSize;
	}
	PlaybackParameters(const PlaybackParameters& copy)
	{
		_hostApi = new std::string(copy.GetHostApi());
		_deviceFormat = new std::string(copy.GetDeviceFormat());
		_deviceName = new std::string(copy.GetDeviceName());
		_samplingRate = copy.GetSamplingRate();
		_numberOfChannels = copy.GetNumberOfChannels();
		_outputBufferFrameSize = copy.GetOutputBufferFrameSize();
	}
	~PlaybackParameters()
	{
		delete _hostApi;
		delete _deviceFormat;
		delete _deviceName;
	}

	std::string& GetHostApi() const { return *_hostApi; }
	std::string& GetDeviceFormat() const { return *_deviceFormat; }
	std::string& GetDeviceName() const { return *_deviceName; }

	unsigned int GetSamplingRate() const { return _samplingRate; }
	unsigned int GetNumberOfChannels() const { return _numberOfChannels; }
	unsigned int GetOutputBufferFrameSize() const { return _outputBufferFrameSize; }

private:

	std::string* _hostApi;
	std::string* _deviceFormat;
	std::string* _deviceName;
	unsigned int _samplingRate;
	unsigned int _numberOfChannels;
	unsigned int _outputBufferFrameSize;
};

