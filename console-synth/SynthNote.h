#ifndef SYNTHNOTE_H
#define SYNTHNOTE_H

#include "Envelope.h"

class SynthNote
{
public:

	SynthNote(int midiNote, const Envelope& envelope);
	~SynthNote();

	const int GetMidiNumber();
	const float GetFrequency();
	float GetEnvelopeLevel(float absoluteTime);

	bool IsEngaged();
	bool HasOutput(float absoluteTime);
	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);

private:

	int  _midiNumber;
	Envelope* _envelope;
};

#endif