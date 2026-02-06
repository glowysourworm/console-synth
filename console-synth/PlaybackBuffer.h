#pragma once

#include <exception>
#include <type_traits>

template <typename TSignal>
concept SignalValue = std::is_floating_point<TSignal>::value;


template <SignalValue TSignal>
class PlaybackBuffer
{
public:

	/// <summary>
	/// Playback stream initializer: the signal value should be a double or a float
	/// </summary>
	/// <param name="numberOfChannels">Number of output channels interleved into the buffer</param>
	/// <param name="maxFrameSize">Max number of output frames (one frame is one sample of all channels)</param>
	/// <param name="samplingRate">Sampling rate for the stream backend</param>
	/// <param name="signalLow">Signal high limit</param>
	/// <param name="signalHigh">Signal low limit</param>
	PlaybackBuffer(unsigned int numberOfChannels, 
				   unsigned int samplingRate,
				   unsigned int maxFrameSize,
				   TSignal signalLow,
				   TSignal signalHigh);
	~PlaybackBuffer();

	unsigned int GetNumberOfChannels() const;
	unsigned int GetMaxNumberOfFrames() const;

	/// <summary>
	/// Returns total size of output buffer in terms of interleved samples. Should be the
	/// (number of channels) x (number of frames)
	/// </summary>
	unsigned int GetTotalBufferSize() const;
	unsigned int GetSamplingRate() const;
	TSignal* GetBuffer() const;

	/// <summary>
	/// Function to set the frame at the specified index, for the specified channel.
	/// </summary>
	/// <param name="sample">Output signal sample for this frame's channel</param>
	/// <param name="frameIndex">zero-based index for the buffer frame</param>
	/// <param name="channelIndex">Zero-based index for the channel</param>
	void SetBufferFrame(TSignal sample, unsigned int frameIndex, unsigned int channelIndex);

private:

	TSignal* _buffer;

	unsigned int _numberOfChannels;
	unsigned int _totalBufferSize;
	unsigned int _samplingRate;

	TSignal _signalLow;
	TSignal _signalHigh;
};

template <SignalValue TSignal>
PlaybackBuffer<TSignal>::PlaybackBuffer(unsigned int numberOfChannels, unsigned int samplingRate, unsigned int maxNumberOfFrames, TSignal signalLow, TSignal signalHigh)
{
	_numberOfChannels = numberOfChannels;
	_totalBufferSize = maxNumberOfFrames * numberOfChannels;
	_samplingRate = samplingRate;

	_signalLow = signalLow;
	_signalHigh = signalHigh;

	_buffer = new TSignal[_totalBufferSize];
}

template <SignalValue TSignal>
PlaybackBuffer<TSignal>::~PlaybackBuffer()
{
	delete[] _buffer;
}

template <SignalValue TSignal>
unsigned int PlaybackBuffer<TSignal>::GetNumberOfChannels() const
{
	return _numberOfChannels;
}

template <SignalValue TSignal>
unsigned int PlaybackBuffer<TSignal>::GetMaxNumberOfFrames() const
{
	return _totalBufferSize / _numberOfChannels;
}

template <SignalValue TSignal>
unsigned int PlaybackBuffer<TSignal>::GetTotalBufferSize() const
{
	return _totalBufferSize;
}

template <SignalValue TSignal>
unsigned int PlaybackBuffer<TSignal>::GetSamplingRate() const
{
	return _samplingRate;
}

template <SignalValue TSignal>
TSignal* PlaybackBuffer<TSignal>::GetBuffer() const
{
	return _buffer;
}

template<SignalValue TSignal>
void PlaybackBuffer<TSignal>::SetBufferFrame(TSignal sample, unsigned int frameIndex, unsigned int channelIndex)
{
	unsigned int absoluteIndex = (frameIndex * _numberOfChannels) + channelIndex;

	if (absoluteIndex >= _totalBufferSize)
		throw new std::exception("Out of range of playback stream:  PlaybackStream.cpp");

	_buffer[absoluteIndex] = sample;
}
