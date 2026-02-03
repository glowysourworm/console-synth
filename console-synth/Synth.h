#ifndef SYNTH_H
#define SYNTH_H

#include <vector>

#include "CombFilter.h"
#include "Compressor.h"
#include "EnvelopeFilter.h"
#include "Mixer.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"

// Class to define static piano notes and store their names / frequencies w.r.t. the SDL keyboard
// defined inputs.
//
class Synth
{

public:
	Synth(const SynthConfiguration& configuration);
	~Synth();

	// Update Configuration
	void SetConfiguration(const SynthConfiguration& configuration);

	// Sets midi notes on / off
	void Set(int midiNumber, bool pressed, double absoluteTime);
	bool IsSet(int midiNumber);

	// Synthesizes a full output at the specified time
	float GetSample(double absoluteTime);

private:

	void Initialize(const SynthConfiguration& configuration);

private:

	std::vector<SynthNote*>* _pianoNotes;

	Mixer* _mixer;

	// Post-processing effects	
	EnvelopeFilter* _envelopeFilter;
	Compressor* _compressor;
	CombFilter* _delay;

	SynthConfiguration* _configuration;
};

#endif

