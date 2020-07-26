#ifndef SYNTH_H
#define SYNTH_H

#include <map>

#include "Constant.h"
#include "SynthNote.h"
#include "ButterworthFilter.h"
#include "Reverb.h"
#include "Envelope.h"

// Class to define static piano notes and store their names / frequencies w.r.t. the SDL keyboard
// defined inputs.
//
class Synth
{

public:
	Synth(SynthNote** pianoNotes, int pianoNotesLength);
	~Synth();

	// Getters
	SynthNote* Get(int keyCode);
	
	// Setters
	void Set(int keyCode, bool pressed, float absoluteTime);

	// Shuts off piano envelope
	void SetDisEngaged(float absoluteTime);

	// Synthesizes a full output at the specified time
	float GetSample(float absoluteTime);

private:

	// Oscillators
	float GenerateSine(float absoluteTime, float frequency);
	float GenerateTriangle(float absoluteTime, float frequency);
	float GenerateSawtooth(float absoluteTime, float frequency);
	float GenerateSquare(float absoluteTime, float frequency);

private:

	SynthNote** _pianoNotes;
	int _pianoNotesLength;

	// Post-processing effects
	ButterworthFilter* _filter;
	Envelope* _filterEnvelope;

	Reverb* _reverb;

	CombFilter* _delay;
};

#endif

