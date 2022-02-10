#ifndef SYNTH_H
#define SYNTH_H

#include <map>
#include <vector>

#include "Constant.h"
#include "SynthNote.h"
#include "ButterworthFilter.h"
#include "Reverb.h"
#include "Envelope.h"
#include "AmplitudeOscillator.h"
#include "Compressor.h"
#include "Mixer.h"


// Class to define static piano notes and store their names / frequencies w.r.t. the SDL keyboard
// defined inputs.
//
class Synth
{

public:
	Synth();
	~Synth();

	// Sets midi notes on / off
	void Set(int midiNumber, bool pressed, float absoluteTime);

	// Synthesizes a full output at the specified time
	float GetSample(float absoluteTime);

private:

	// Oscillators
	float GenerateSine(float absoluteTime, float frequency);
	float GenerateTriangle(float absoluteTime, float frequency);
	float GenerateSawtooth(float absoluteTime, float frequency);
	float GenerateSquare(float absoluteTime, float frequency);

private:

	std::vector<SynthNote*>* _pianoNotes;

	float _frequencyShift;
	float _frequencyShiftGain;

	Mixer* _mixer;

	// Post-processing effects
	AmplitudeOscillator* _oscillator;
	ButterworthFilter* _filter;
	Envelope* _filterEnvelope;
	Compressor* _limiter;
	Reverb* _reverb;
	CombFilter* _delay;
};

#endif

