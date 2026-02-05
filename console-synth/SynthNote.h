#ifndef SYNTHNOTE_H
#define SYNTHNOTE_H

#include "AmplitudeOscillator.h"
#include "Compressor.h"
#include "Envelope.h"
#include "EnvelopeFilter.h"
#include "SynthConfiguration.h"

class SynthNote
{
public:

	SynthNote(int midiNumber, const SynthConfiguration& configuration);
	~SynthNote();

	int GetMidiNumber() const;
	float GetFrequency() const;
	float GetSample(float absoluteTime) const;


	bool IsEngaged();
	bool HasOutput(float absoluteTime);
	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);

private:

	int  _midiNumber;
	Envelope* _envelope;
	EnvelopeFilter* _envelopeFilter;
	AmplitudeOscillator* _oscillator; 
	Compressor* _compressor;

	bool _envelopeFilterEnabled;
	bool _compressorEnabled;
};

#endif