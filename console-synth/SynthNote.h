#ifndef SYNTHNOTE_H
#define SYNTHNOTE_H

#include "WindowsKeyCodes.h"
#include "Envelope.h"
#include <math.h>
#include <atomic>

class SynthNote
{
public:

	SynthNote(int midiNumber);
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