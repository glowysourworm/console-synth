#pragma once

#include "PlaybackBuffer.h"
#include "PlaybackDevice.h"
#include "PlaybackParameters.h"
#include "Synth.h"
#include "SynthConfiguration.h"

template<SignalValue TSignal>
class MidiPlaybackDevice : public PlaybackDevice<TSignal>
{
public:

	bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) override;

	int WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration) override;
	//int WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackBuffer, unsigned int numberOfFrames, double streamTime) override;

private:

	Synth* _synth;
	int _frameIndex;
	bool _initialized;
	float* _waveBuffer;
	int _waveBufferLength;

};

template<SignalValue TSignal>
bool MidiPlaybackDevice<TSignal>::Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters)
{
	_initialized = true;

	//MidiFile midi;

	//midi.read(std::string(fileName));

	//// Set "absolute ticks mode"
	//midi.absoluteTicks();

	//// Finally, try merging tracks to create a single-track song
	//// midiFile.joinTracks();

	//float secondsPerTick = midi.getFileDurationInSeconds() / midi.getFileDurationInTicks();
	//float timeCursor = 0;

	//// Calculate number of midi ticks to progress during this callback
	//// double midiTicksPerFrame = (((1 / (double)NUMBER_CHANNELS) / (double)SAMPLING_RATE) * ticksPerSecond);

	//_waveBufferLength = midi.getFileDurationInSeconds() * SAMPLING_RATE;
	//int waveIndex = 0;
	//int samplesPerTick = (int)(secondsPerTick * SAMPLING_RATE);

	//_waveBuffer = new float[_waveBufferLength];

	//for (int k = 0; k < midi.getFileDurationInTicks(); k++)
	//{
	//	for (int i = 0; i < midi.getTrackCount(); i++)
	//	{
	//		if (k >= midi[i].size())
	//			continue;

	//		MidiEvent midiEvent = midi[i][k];

	//		// Note On
	//		if (midiEvent.isNoteOn())
	//		{
	//			_synth->Set(midiEvent.getKeyNumber(), true, timeCursor);
	//		}

	//		// Note Off
	//		else if (midiEvent.isNoteOff())
	//		{
	//			_synth->Set(midiEvent.getKeyNumber(), false, timeCursor);
	//		}
	//	}

	//	// Advance midi time clock
	//	timeCursor += secondsPerTick;

	//	// Synthesize samples during that time period
	//	for (int i = waveIndex; (i < (waveIndex + samplesPerTick)) && (i < _waveBufferLength); i++)
	//	{
	//		_waveBuffer[i] = _synth->GetSample(i / (float)SAMPLING_RATE);
	//	}

	//	// Increment wave index
	//	waveIndex += samplesPerTick;
	//}

	return _initialized;
}

template<SignalValue TSignal>
int MidiPlaybackDevice<TSignal>::WritePlaybackBuffer(void* playbackBuffer, unsigned int numberOfFrames, double streamTime, const SynthConfiguration* configuration)
{
	return 0;
}
//
//template<SignalValue TSignal>
//int MidiPlaybackDevice<TSignal>::WritePlaybackBuffer(PlaybackBuffer<TSignal>* playbackStream, unsigned int numberOfFrames, double streamTime)
//{
//	//if (!_initialized)
//	//	return 0;
//
//	//if (status == RTAUDIO_INPUT_OVERFLOW)
//	//{
//
//	//}
//	//else if (status == RTAUDIO_OUTPUT_UNDERFLOW)
//	//{
//
//	//}
//
//	//// Output frames should be interleved
//	//float* buffer = (float*)outputBuffer;
//	//float sampleSize = (1.0 / (float)SAMPLING_RATE);
//	//float systemTime = (float)_frameIndex / (float)SAMPLING_RATE;
//
//	//// Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
//	//for (unsigned int i = 0; (i < nFrames) && (_frameIndex < _waveBufferLength); i++)
//	//{
//	//	float absoluteTime = (_frameIndex++) * sampleSize;
//	//	// float sample = _synth->GetSample(absoluteTime);
//	//	float sample = _waveBuffer[_frameIndex + i];
//
//	//	// Interleved frames
//	//	for (unsigned int j = 0; j < NUMBER_CHANNELS; j++)
//	//	{
//	//		// Initialize sample to zero
//	//		buffer[(2 * i) + j] = sample;
//	//	}
//	//}
//
//	return 0;
//}