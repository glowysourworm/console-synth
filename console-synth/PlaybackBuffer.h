#pragma once

#include <Windows.h>
#include <bit>
#include <cstdint>
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
	/// <param name="frameByteSize">Size of frame in bytes</param>
	/// <param name="signalLow">Signal high limit</param>
	/// <param name="signalHigh">Signal low limit</param>
	PlaybackBuffer(unsigned int numberOfChannels, 
				   unsigned int samplingRate,
				   unsigned int maxFrameSize,
				   unsigned int frameByteSize,
				   TSignal signalLow,
				   TSignal signalHigh);
	~PlaybackBuffer();

	unsigned int GetNumberOfChannels() const;
	unsigned int GetMaxNumberOfFrames() const;

	unsigned int GetSamplingRate() const;
	BYTE* GetBuffer() const;

	/// <summary>
	/// Function to set the frame at the specified index, for the specified channel.
	/// </summary>
	/// <param name="sample">Output signal sample for this frame's channel</param>
	/// <param name="frameIndex">zero-based index for the buffer frame</param>
	/// <param name="channelIndex">Zero-based index for the channel</param>
	void SetBufferFrame(TSignal sample, unsigned int frameIndex, unsigned int channelIndex);

private:

	BYTE* _buffer;

	unsigned int _numberOfChannels;
	unsigned int _totalBufferSize;
	unsigned int _samplingRate;
	unsigned int _frameByteSize;

	TSignal _signalLow;
	TSignal _signalHigh;
};

template <SignalValue TSignal>
PlaybackBuffer<TSignal>::PlaybackBuffer(unsigned int numberOfChannels,
										unsigned int samplingRate,
										unsigned int maxNumberOfFrames,
										unsigned int frameByteSize,
										TSignal signalLow, TSignal signalHigh)
{
	_numberOfChannels = numberOfChannels;
	_totalBufferSize = maxNumberOfFrames * frameByteSize;
	_samplingRate = samplingRate;
	_frameByteSize = frameByteSize;

	_signalLow = signalLow;
	_signalHigh = signalHigh;

	_buffer = new BYTE[_totalBufferSize];
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
unsigned int PlaybackBuffer<TSignal>::GetSamplingRate() const
{
	return _samplingRate;
}

template <SignalValue TSignal>
BYTE* PlaybackBuffer<TSignal>::GetBuffer() const
{
	return _buffer;
}

template<SignalValue TSignal>
void PlaybackBuffer<TSignal>::SetBufferFrame(TSignal sample, unsigned int frameIndex, unsigned int channelIndex)
{
	unsigned int absoluteIndex = (frameIndex * _frameByteSize) + (channelIndex * (_frameByteSize / 2));

	if (absoluteIndex + 3 >= _totalBufferSize)
		throw new std::exception("Out of range of playback stream:  PlaybackStream.cpp");

	uint32_t outputSample = std::bit_cast<uint32_t, TSignal>(sample);

	BYTE byte1 = (BYTE)(outputSample & 0x000000FF);
	BYTE byte2 = (BYTE)((outputSample & 0x0000FF00) >> 8);
	BYTE byte3 = (BYTE)((outputSample & 0x00FF0000) >> 16);
	BYTE byte4 = (BYTE)((outputSample & 0xFF000000) >> 24);

	if (byte4 > 0)
	{
		int foo = 4;
	}

	// Output Sample Bytes
	_buffer[absoluteIndex] = byte1;
	_buffer[absoluteIndex + 1] = byte2;
	_buffer[absoluteIndex + 2] = byte3;
	_buffer[absoluteIndex + 3] = byte4;

	// Input Sample Bytes (would go here for full duplex)
}
