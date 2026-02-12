#pragma once
#include <cmath>
#include <exception>

class PlaybackFrame
{	
public:

	PlaybackFrame(int numberChannels)
	{
		_numberOfChannels = numberChannels;
		_frameSamples = new float[numberChannels];

		for (int index = 0; index < numberChannels; index++)
		{
			_frameSamples[index] = 0.0f;
		}
	}
	~PlaybackFrame()
	{
		delete[] _frameSamples;
	}

	/// <summary>
	/// Sets all samples in the frame to this sample
	/// </summary>
	void SetFrame(float sample)
	{
		for (int index = 0; index < _numberOfChannels; ++index)
		{
			_frameSamples[index] = sample;
		}
	}

	/// <summary>
	/// Sets frame according to channel inputs. Any excess channels are set to channel1
	/// </summary>
	void SetFrame(float channel0, float channel1)
	{
		if (_numberOfChannels > 0)
			_frameSamples[0] = channel0;

		if (_numberOfChannels > 1)
			_frameSamples[1] = channel1;

		for (int index = 2; index < _numberOfChannels; index++)
		{
			_frameSamples[index] = channel1;
		}
	}

	void SetFrame(const PlaybackFrame* copy)
	{
		if (_numberOfChannels != copy->GetChannelCount())
			throw new std::exception("Mismatching channel count:  PlaybackFrame.h");

		for (int index = 0; index < _numberOfChannels; index++)
		{
			_frameSamples[index] = copy->GetSample(index);
		}
	}

	void AddFrame(const PlaybackFrame* copy)
	{
		if (_numberOfChannels != copy->GetChannelCount())
			throw new std::exception("Mismatching channel count:  PlaybackFrame.h");

		for (int index = 0; index < _numberOfChannels; index++)
		{
			_frameSamples[index] += copy->GetSample(index);
		}
	}

	void SetSample(int channel, float sample)
	{
		_frameSamples[channel] = sample;
	}

	void Clear()
	{
		for (int index = 0; index < _numberOfChannels; index++)
		{
			_frameSamples[index] = 0;
		}
	}

	float GetSample(int channel) const
	{
		return _frameSamples[channel];
	}

	unsigned int GetChannelCount() const
	{
		return _numberOfChannels;
	}

private:

	unsigned int _numberOfChannels;
	float* _frameSamples;
};