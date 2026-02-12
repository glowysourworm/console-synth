#pragma once
#include "Constant.h"
#include "Envelope.h"
#include "SynthNoteMap.h"
#include "WindowsKeyCodes.h"
#include <atomic>
#include <thread>

class SynthConfiguration
{
public:

	SynthConfiguration();
	SynthConfiguration(const SynthConfiguration& copy);
	~SynthConfiguration();	

	/// <summary>
	/// Returns an std::atomic provided waiting indicator which should be thread-safe.
	/// </summary>
	bool IsWaiting() const;
	bool IsDirty() const;

	/// <summary>
	/// Attempts to set the std::atomic wait flag. If the user code does not synchronize, the
	/// acquire will fail, and return false. So, your wait loop should ask IsWaiting, then double
	/// check that wait sychronization was acquired. If you're clearing the wait flag, the return
	/// value must ALSO be true!
	/// </summary>
	bool SetWait(bool desiredValue);
	void ClearDirty();

	void SetOscillatorType(OscillatorType value);
	void SetEnvelopeFilterType(EnvelopeFilterType value);	
	void SetEnvelopeFilterOscillatorType(OscillatorType value);

	void SetMidiLow(int value);
	void SetMidiHigh(int value);

	void SetMidiNote(WindowsKeyCodes keyCode, int midiNote);

	void SetHasReverb(bool value);
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

	void SetReverbSeconds(float value);
	void SetReverbGain(float value);
	void SetReverbWetDry(float value);

	void SetDelaySeconds(float value);
	void SetDelayGain(float value);
	void SetDelayFeedback(bool value);
	void SetDelayWetDry(float value);

	void SetOutputLeftRight(float value);
	void SetOutputGain(float value);

	int GetMidiLow() const;
	int GetMidiHigh() const;

	SynthNoteMap GetNoteMap() const;
	bool HasMidiNote(WindowsKeyCodes keyCode) const;
	int GetMidiNote(WindowsKeyCodes keyCode) const;
	WindowsKeyCodes GetKeyCode(int midiNote) const;

	OscillatorType GetOscillatorType() const;

	bool GetHasReverb() const;
	bool GetHasDelay() const;
	bool GetHasCompressor() const;
	bool GetHasEnvelopeFilter() const;

	Envelope GetNoteEnvelope() const;
	Envelope GetEnvelopeFilter() const;

	int GetEnvelopeFilterCutoff() const;
	float GetEnvelopeFilterResonance() const;
	EnvelopeFilterType GetEnvelopeFilterType() const;
	OscillatorType GetEnvelopeFilterOscillatorType() const;
	float GetEnvelopeFilterOscillatorFrequency() const;

	float GetCompressorThreshold() const;
	float GetCompressionRatio() const;
	float GetCompressorRelaxationPeriod() const;
	float GetCompressorAttack() const;
	float GetCompressorRelease() const;
	float GetCompressorGain() const;

	float GetReverbDelaySeconds() const;
	float GetReverbGain() const;
	float GetReverbWetDry() const;

	float GetDelaySeconds() const;
	float GetDelayGain() const;
	bool GetDelayFeedback() const;
	float GetDelayWetDry() const;

	float GetOutputLeftRight() const;
	float GetOutputGain() const;

public:

	void IterateKeymap(SynthNoteMap::KeymapIterationCallback callback) const;

private:

	SynthNoteMap* _keyMap;

	// Using an std::atomic for thread synchronization
	std::atomic<bool> _waitFlag;

	// Using, also, a thread ID to manage the wait flag
	std::thread::id _threadId;

	// Tracks changes to the configuration
	bool _isDirty;

	int _midiLow;
	int _midiHigh;

	OscillatorType _oscillatorType;
	EnvelopeFilterType _envelopeFilterType;

	bool _hasReverb;
	bool _hasDelay;
	bool _hasCompressor;
	bool _hasEnvelopeFilter;

	Envelope* _noteEnvelope;

	Envelope* _envelopeFilter;
	OscillatorType _envelopeFilterOscillatorType;
	int _envelopeFilterCutoff;
	float _envelopeFilterResonance;	
	float _envelopeFilterOscillatorFrequency;

	// Compressor
	float _compressorGain;
	float _compressorThreshold;
	float _compressionRatio;
	float _compressorRelaxationPeriod;
	float _compressorAttack;
	float _compressorRelease;

	// Reverb
	float _reverbDelaySeconds;
	float _reverbGain;
	float _reverbWetDry;

	// Delay
	float _delaySeconds;
	float _delayGain;
	float _delayWetDry;
	bool _delayFeedback;

	// Output
	float _leftRight;
	float _gain;
};

