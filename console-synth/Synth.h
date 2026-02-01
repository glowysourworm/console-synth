#ifndef SYNTH_H
#define SYNTH_H

#include <vector>

#include "AmplitudeOscillator.h"
#include "ButterworthFilter.h"
#include "CombFilter.h"
#include "Compressor.h"
#include "Envelope.h"
#include "Mixer.h"
#include "Reverb.h"
#include "SynthNote.h"


// Class to define static piano notes and store their names / frequencies w.r.t. the SDL keyboard
// defined inputs.
//
class Synth
{

public:
	Synth();
	~Synth();

	// Sets midi notes on / off
	void Set(int midiNumber, bool pressed, double absoluteTime);
	bool IsSet(int midiNumber);

	// Synthesizes a full output at the specified time
	float GetSample(double absoluteTime);

private:

	// Oscillators
	float GenerateSine(double absoluteTime, float frequency);
	float GenerateTriangle(double absoluteTime, float frequency);
	float GenerateSawtooth(double absoluteTime, float frequency);
	float GenerateSquare(double absoluteTime, float frequency);

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

