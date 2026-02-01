#ifndef SYNTHNOTE_H
#define SYNTHNOTE_H

#include "Envelope.h"

class SynthNote
{
public:
	SynthNote(int m);
	~SynthNote();

	const int GetMidiNumber();
	const float GetFrequency();
	float GetEnvelopeLevel(float absoluteTime);

	bool IsEngaged();
	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);

private:

	int  _midiNumber;
	Envelope* _envelope;
};

#endif