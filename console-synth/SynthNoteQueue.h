#pragma once

#include "PlaybackFrame.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"
#include <map>
#include <vector>

class SynthNoteQueue
{
public:

	SynthNoteQueue(int capacity);
	~SynthNoteQueue();

	/// <summary>
	/// Sets midi note to either engaged / disengaged. Returns true if there are enough voice slots
	/// to add the next note, false otherwise.
	/// </summary>
	/// <param name="midiNumber">MIDI number of the desired note</param>
	/// <param name="pressed">Engaged / Disengaged</param>
	/// <param name="absoluteTime">Current stream time</param>
	/// <returns>True if the note was engaged properly. Disengaged notes will be kept until the output has ceased.</returns>
	bool SetNote(int midiNumber, bool pressed, double absoluteTime, const SynthConfiguration* configuration, unsigned int samplingRate) const;

	/// <summary>
	/// Synthesizes a full output at the specified stream time. Returns true if there was output this call.
	/// </summary>
	bool SetSample(PlaybackFrame* frame, double absoluteTime, double gain, double leftRight);

private:

	int _capacity;

	// Capacity-sized map, will hold notes up to the user capacity (should be 10, for 10 active voices)
	std::map<int, SynthNote*>* _engagedNotes;

	// N-sized map, will hold notes until they've dissipated
	std::vector<SynthNote*>* _disengagedNotes;

};

