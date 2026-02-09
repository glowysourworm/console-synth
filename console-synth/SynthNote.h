#ifndef SYNTHNOTE_H
#define SYNTHNOTE_H

#include "CompressorChannel.h"
#include "Envelope.h"
#include "EnvelopeFilterChannel.h"
#include "OscillatorBase.h"
#include "PlaybackFrame.h"
#include "SynthConfiguration.h"

class SynthNote
{
public:

	SynthNote(int midiNumber, const SynthConfiguration& configuration, unsigned int samplingRate);
	~SynthNote();

	int GetMidiNumber() const;
	float GetFrequency() const;
	void GetSample(PlaybackFrame* frame, float absoluteTime) const;
	void AddSample(PlaybackFrame* frame, float absoluteTime) const;


	bool IsEngaged();
	bool HasOutput(float absoluteTime);
	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);

private:

	/// <summary>
	/// Applys note to the playback frame either by overwriting or adding
	/// </summary>
	void ApplyImpl(PlaybackFrame* frame, float absoluteTime, bool overwriteOrAdd) const;

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