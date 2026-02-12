#pragma once

#include "AirwindowsEffect.h"
#include "Filter.h"
#include "PlaybackFrame.h"
#include "SynthConfiguration.h"
#include "SynthNoteQueue.h"

// Class to define static piano notes and store their names / frequencies w.r.t. the SDL keyboard
// defined inputs.
//
class Synth
{

public:
	Synth(const SynthConfiguration* configuration, unsigned int numberOfChannels, unsigned int samplingRate);
	~Synth();

	// Update Configuration
	void SetConfiguration(const SynthConfiguration* configuration);

	// Sets midi notes on / off
	void Set(int midiNumber, bool pressed, double absoluteTime, const SynthConfiguration* configuration);

	/// <summary>
	/// Synthesizes a full output at the specified stream time. Returns true if there was output this call.
	/// </summary>
	bool GetSample(PlaybackFrame* frame, double absoluteTime, const SynthConfiguration* configuration);

private:

	// Synth Notes by Midi Number
	SynthNoteQueue* _pianoNotes;

	// Post-processing effects	
	//Reverb* _reverb;
	AirwindowsEffect* _reverb;
	Filter* _delay;

	unsigned int _numberOfChannels;
	unsigned int _samplingRate;
};