#ifndef SYNTH_H
#define SYNTH_H

#include <map>

#include "AirwindowsEffect.h"
#include "Constant.h"
#include "Filter.h"
#include "Mixer.h"
#include "PlaybackFrame.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"

// Class to define static piano notes and store their names / frequencies w.r.t. the SDL keyboard
// defined inputs.
//
class Synth
{

public:
	Synth(const SynthConfiguration& configuration, unsigned int numberOfChannels, unsigned int samplingRate);
	~Synth();

	// Update Configuration
	void SetConfiguration(const SynthConfiguration& configuration);

	// Sets midi notes on / off
	void Set(int midiNumber, bool pressed, double absoluteTime);

	/// <summary>
	/// Returns true if there is output from the midi numbered synth note
	/// </summary>
	bool IsSet(int midiNumber);

	/// <summary>
	/// Returns true if there is an active note for this midi number
	/// </summary>
	bool HasNote(int midiNumber);

	/// <summary>
	/// Synthesizes a full output at the specified stream time
	/// </summary>
	void GetSample(PlaybackFrame* frame, double absoluteTime);

	/// <summary>
	/// Clears out notes that have no ouptut
	/// </summary>
	void Clear(double absoluteTime);

public:

	/// <summary>
	/// Iterates currently active notes in the synth
	/// </summary>
	void GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const;

private:

	void Initialize(const SynthConfiguration& configuration);

private:

	// Synth Notes by Midi Number
	std::map<int, SynthNote*>* _pianoNotes;

	Mixer* _mixer;

	// Post-processing effects	
	//Reverb* _reverb;
	AirwindowsEffect* _reverb;
	Filter* _delay;

	SynthConfiguration* _configuration;
	unsigned int _numberOfChannels;
	unsigned int _samplingRate;
};

#endif

