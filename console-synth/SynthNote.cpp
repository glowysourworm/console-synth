#include "Envelope.h"
#include "SynthNote.h"
#include <cmath>

SynthNote::SynthNote(int midiNumber)
{
	_midiNumber = midiNumber;
	_envelope = new Envelope(0.01, 0.01, 0.01, 0.01, 1, 1);
}

SynthNote::~SynthNote()
{
	delete _envelope;
}

const int SynthNote::GetMidiNumber()
{
	return _midiNumber;
}

float SynthNote::GetEnvelopeLevel(float absoluteTime)
{
	return _envelope->GetEnvelopeLevel(absoluteTime);
}

const float SynthNote::GetFrequency()
{
	return 440 * powf(2, ((_midiNumber - 69.0) / 12.0));
}

bool SynthNote::IsEngaged()
{
	return _envelope->IsEngaged();
}

void SynthNote::Engage(float absoluteTime)
{
	_envelope->Engage(absoluteTime);
}

void SynthNote::DisEngage(float absoluteTime)
{
	_envelope->DisEngage(absoluteTime);
}