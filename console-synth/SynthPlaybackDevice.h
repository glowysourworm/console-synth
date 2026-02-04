#pragma once

#include "Constant.h"
#include "PlaybackDevice.h"
#include "RtAudio.h"
#include "Synth.h"
#include "SynthConfiguration.h"

class SynthPlaybackDevice : public PlaybackDevice
{
public:

	bool Initialize() override;

	int RtAudioCallback(void* outputBuffer,
		void* inputBuffer,
		unsigned int nFrames,
		double streamTime,
		RtAudioStreamStatus status,
		void* userData) override;

	void SetNote(int midiNumber, bool pressed, double streamTime);
	bool GetNote(int midiNumber) const;
	bool HasNote(int midiNumber) const;
	void ClearUnused(double streamTime);
	void UpdateSynth(const SynthConfiguration& configuration);

public:

	/// <summary>
	/// Gets the currently active notes in the synth as an array of midi note numbers
	/// </summary>
	void GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const;

private:

	Synth* _synth;
	bool _initialized;
};

