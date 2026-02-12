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

		_streamTime = 0;
		_averageUIMilli = 0;
		_averageFrontendMilli = 0;
		_averageAudioMilli = 0;
		_streamLatency = 0;
	}
	PlaybackParameters(const PlaybackParameters& copy)
	{
		_hostApi = new std::string(copy.GetHostApi());
		_deviceFormat = new std::string(copy.GetDeviceFormat());
		_deviceName = new std::string(copy.GetDeviceName());
		_samplingRate = copy.GetSamplingRate();
		_numberOfChannels = copy.GetNumberOfChannels();
		_outputBufferFrameSize = copy.GetOutputBufferFrameSize();

		_streamTime = copy.GetStreamTime();
		_averageUIMilli = copy.GetAvgUIMilli();
		_averageFrontendMilli = copy.GetAvgAudioMilli();
		_averageAudioMilli = copy.GetAvgFrontendMilli();
		_streamLatency = copy.GetStreamLatency();
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

	float GetStreamTime() const { return _streamTime; }
	float GetAvgUIMilli() const { return _averageUIMilli; }
	float GetAvgAudioMilli() const { return _averageAudioMilli; }
	float GetAvgFrontendMilli() const { return _averageFrontendMilli; }
	long GetStreamLatency() const { return _streamLatency; }

	void UpdateDevice(const std::string& hostApi, 
					  const std::string& deviceFormat, 
					  const std::string& deviceName, 
					  unsigned int samplingRate, 
					  unsigned int numberChannels, 
					  unsigned int bufferFrameSize)
	{
		_hostApi = new std::string(hostApi);
		_deviceFormat = new std::string(deviceFormat);
		_deviceName = new std::string(deviceName);
		_samplingRate = samplingRate;
		_numberOfChannels = numberChannels;
		_outputBufferFrameSize = bufferFrameSize;
	}
	void UpdateRT(float streamTime, float avgUIMilli, float avgAudioMilli, float avgFrontendMilli, long latency)
	{
		_streamTime = streamTime;
		_averageUIMilli = avgUIMilli;
		_averageFrontendMilli = avgFrontendMilli;
		_averageAudioMilli = avgAudioMilli;
		_streamLatency = latency;
	}

private:

	std::string* _hostApi;
	std::string* _deviceFormat;
	std::string* _deviceName;
	unsigned int _samplingRate;
	unsigned int _numberOfChannels;
	unsigned int _outputBufferFrameSize;

	float _streamTime;
	float _averageUIMilli;
	float _averageAudioMilli;
	float _averageFrontendMilli;
	long _streamLatency;
};

