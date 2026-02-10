#ifndef SYNTHNOTE_H
#define SYNTHNOTE_H

#include "CompressorChannel.h"
#include "Constant.h"
#include "Envelope.h"
#include "EnvelopeFilterChannel.h"
#include "OscillatorBase.h"
#include "PlaybackFrame.h"
#include "SynthConfiguration.h"

class SynthNote
{
public:

	SynthNote(const SynthConfiguration* configuration, int midiNumber, unsigned int samplingRate);
	~SynthNote();

	void SetConfiguration(const SynthConfiguration* configuration);

	int GetMidiNumber() const;
	float GetFrequency() const;
	void GetSample(PlaybackFrame* frame, float absoluteTime) const;


	bool IsEngaged();
	bool HasOutput(float absoluteTime);
	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);

private:

	void CreateOscillator(OscillatorType type);

private:

	int  _midiNumber;
	OscillatorBase* _oscillator;
	Envelope* _envelope;
	EnvelopeFilterChannel* _envelopeFilter;
	CompressorChannel* _compressor;

	bool _envelopeFilterEnabled;
	bool _compressorEnabled;
};

#endif