#pragma once
#include "Constant.h"
#include "Envelope.h"
#include "SynthNoteMap.h"
#include "WindowsKeyCodes.h"

class SynthConfiguration
{
public:

	SynthConfiguration();
	SynthConfiguration(const SynthConfiguration& copy);
	~SynthConfiguration();	

	bool IsDirty() const;

	void ClearDirty();

	void SetOscillatorType(AmplitudeOscillatorType value);
	void SetEnvelopeFilterType(EnvelopeFilterType value);	
	void SetEnvelopeFilterOscillatorType(AmplitudeOscillatorType value);

	void SetMidiLow(int value);
	void SetMidiHigh(int value);

	void SetMidiNote(WindowsKeyCodes keyCode, int midiNote);

	void SetHasDelay(bool value);
	void SetHasCompressor(bool value);
	void SetHasEnvelopeFilter(bool value);

	void SetNoteEnvelope(const Envelope& value);
	void SetEnvelopeFilter(const Envelope& value);
	void SetEnvelopeFilterCutoff(int value);
	void SetEnvelopeFilterResonance(float value);
	void SetEnvelopeFilterOscillatorFrequency(float value);

	void SetCompressorThreshold(float value);
	void SetCompressionRatio(float value);
	void SetCompressorRelaxationPeriod(float value);
	void SetCompressorAttack(float value);
	void SetCompressorRelease(float value);
	void SetCompressorGain(float value);

	void SetDelaySeconds(float value);
	void SetDelayGain(float value);
	void SetDelayFeedback(bool value);

	int GetMidiLow() const;
	int GetMidiHigh() const;

	bool HasMidiNote(WindowsKeyCodes keyCode) const;
	int GetMidiNote(WindowsKeyCodes keyCode) const;
	WindowsKeyCodes GetKeyCode(int midiNote) const;

	AmplitudeOscillatorType GetOscillatorType() const;

	bool GetHasDelay() const;
	bool GetHasCompressor() const;
	bool GetHasEnvelopeFilter() const;

	Envelope GetNoteEnvelope() const;
	Envelope GetEnvelopeFilter() const;

	int GetEnvelopeFilterCutoff() const;
	float GetEnvelopeFilterResonance() const;
	EnvelopeFilterType GetEnvelopeFilterType() const;
	AmplitudeOscillatorType GetEnvelopeFilterOscillatorType() const;
	float GetEnvelopeFilterOscillatorFrequency() const;

	float GetCompressorThreshold() const;
	float GetCompressionRatio() const;
	float GetCompressorRelaxationPeriod() const;
	float GetCompressorAttack() const;
	float GetCompressorRelease() const;
	float GetCompressorGain() const;

	float GetDelaySeconds() const;
	float GetDelayGain() const;
	bool GetDelayFeedback() const;

public:

	void IterateKeymap(SynthNoteMap::KeymapIterationCallback callback) const;

private:

	SynthNoteMap* _keyMap;

	// Tracks changes to the configuration
	bool _isDirty;

	int _midiLow;
	int _midiHigh;

	AmplitudeOscillatorType _oscillatorType;
	EnvelopeFilterType _envelopeFilterType;

	bool _hasDelay;
	bool _hasCompressor;
	bool _hasEnvelopeFilter;

	Envelope* _noteEnvelope;

	Envelope* _envelopeFilter;
	AmplitudeOscillatorType _envelopeFilterOscillatorType;
	int _envelopeFilterCutoff;
	float _envelopeFilterResonance;	
	float _envelopeFilterOscillatorFrequency;

	float _compressorGain;
	float _compressorThreshold;
	float _compressionRatio;
	float _compressorRelaxationPeriod;
	float _compressorAttack;
	float _compressorRelease;

	float _delaySeconds;
	float _delayGain;
	bool _delayFeedback;
};

