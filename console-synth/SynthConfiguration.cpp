#include "Constant.h"
#include "Envelope.h"
#include "SynthConfiguration.h"

SynthConfiguration::SynthConfiguration()
{
	_isDirty = false;

	_oscillatorType = AmplitudeOscillatorType::Sine;

	_midiLow = MIDI_PIANO_LOW_NUMBER;
	_midiHigh = MIDI_PIANO_HIGH_NUMBER;

	_hasDelay = false;
	_hasCompressor = false;
	_hasEnvelopeFilter = false;

	_noteEnvelope = new Envelope();
	_envelopeFilter = new Envelope();

	_envelopeFilterCutoff = 5000;
	_envelopeFilterResonance = 1;

	_compressorThreshold = 0.5;
	_compressionRatio = 1;
	_compressorRelaxationPeriod = 1;
	_compressorAttack = 0.2;
	_compressorRelease = 0.3;

	_delaySeconds = 0.5;
	_delayFeedback = false;
}
SynthConfiguration::SynthConfiguration(const SynthConfiguration& copy)
{
	if (_noteEnvelope != nullptr)
		delete _noteEnvelope;

	if (_envelopeFilter != nullptr)
		delete _envelopeFilter;

	_midiLow = copy.GetMidiLow();
	_midiHigh = copy.GetMidiHigh();

	_oscillatorType = copy.GetOscillatorType();

	_hasDelay = copy.GetHasDelay();
	_hasCompressor = copy.GetHasCompressor();
	_hasEnvelopeFilter = copy.GetHasEnvelopeFilter();

	_noteEnvelope = new Envelope(copy.GetNoteEnvelope());
	_envelopeFilter = new Envelope(copy.GetEnvelopeFilter());

	_envelopeFilterCutoff = copy.GetEnvelopeFilterCutoff();
	_envelopeFilterResonance = copy.GetEnvelopeFilterResonance();

	_compressorThreshold = copy.GetCompressorThreshold();
	_compressionRatio = copy.GetCompressionRatio();
	_compressorRelaxationPeriod = copy.GetCompressorRelaxationPeriod();
	_compressorAttack = copy.GetCompressorAttack();
	_compressorRelease = copy.GetCompressorRelease();

	_delaySeconds = copy.GetDelaySeconds();
	_delayFeedback = copy.GetDelayFeedback();
}
SynthConfiguration::~SynthConfiguration()
{
	if (_noteEnvelope != nullptr)
		delete _noteEnvelope;

	if (_envelopeFilter != nullptr)
		delete _envelopeFilter;
}

bool SynthConfiguration::IsDirty() const
{
	return _isDirty;
}

void SynthConfiguration::ClearDirty()
{
	_isDirty = false;
}

AmplitudeOscillatorType SynthConfiguration::GetOscillatorType() const
{
	return _oscillatorType;
}
bool SynthConfiguration::GetHasDelay() const
{
	return _hasDelay;
}
bool SynthConfiguration::GetHasCompressor() const
{
	return _hasCompressor;
}
bool SynthConfiguration::GetHasEnvelopeFilter() const
{
	return _hasEnvelopeFilter;
}

Envelope SynthConfiguration::GetNoteEnvelope() const
{
	return *_noteEnvelope;
}
Envelope SynthConfiguration::GetEnvelopeFilter() const
{
	return *_envelopeFilter;
}

int SynthConfiguration::GetEnvelopeFilterCutoff() const
{
	return _envelopeFilterCutoff;
}
float SynthConfiguration::GetEnvelopeFilterResonance() const
{
	return _envelopeFilterResonance;
}
float SynthConfiguration::GetCompressorThreshold() const
{
	return _compressorThreshold;
}
float SynthConfiguration::GetCompressionRatio() const
{
	return _compressionRatio;
}
float SynthConfiguration::GetCompressorRelaxationPeriod() const
{
	return _compressorRelaxationPeriod;
}
float SynthConfiguration::GetCompressorAttack() const
{
	return _compressorAttack;
}
float SynthConfiguration::GetCompressorRelease() const
{
	return _compressorRelease;
}
float SynthConfiguration::GetDelaySeconds() const
{
	return _delaySeconds;
}
bool SynthConfiguration::GetDelayFeedback() const
{
	return _delayFeedback;
}
int SynthConfiguration::GetMidiLow() const
{
	return _midiLow;
}
int SynthConfiguration::GetMidiHigh() const
{
	return _midiHigh;
}

void SynthConfiguration::SetOscillatorType(AmplitudeOscillatorType value)
{
	if (_oscillatorType != value)
		_isDirty = true;

	_oscillatorType = value;
}
void SynthConfiguration::SetMidiLow(int value)
{
	if (_midiLow != value)
		_isDirty = true;

	_midiLow = value;
}
void SynthConfiguration::SetMidiHigh(int value)
{
	if (_midiHigh != value)
		_isDirty = true;

	_midiHigh = value;
}
void SynthConfiguration::SetHasDelay(bool value)
{
	if (_hasDelay != value)
		_isDirty = true;

	_hasDelay = value;
}
void SynthConfiguration::SetHasCompressor(bool value)
{
	if (_hasCompressor != value)
		_isDirty = true;

	_hasCompressor = value;
}
void SynthConfiguration::SetHasEnvelopeFilter(bool value)
{
	if (_hasEnvelopeFilter != value)
		_isDirty = true;

	_hasEnvelopeFilter = value;
}

void SynthConfiguration::SetNoteEnvelope(const Envelope& value)
{
	if (*_noteEnvelope != value)
	{
		_isDirty = true;

		delete _noteEnvelope;

		_noteEnvelope = new Envelope(value);
	}
}
void SynthConfiguration::SetEnvelopeFilter(const Envelope& value)
{
	if (*_envelopeFilter != value)
	{
		_isDirty = true;

		delete _envelopeFilter;

		_envelopeFilter = new Envelope(value);
	}
}
void SynthConfiguration::SetEnvelopeFilterCutoff(int value)
{
	if (_envelopeFilterCutoff != value)
		_isDirty = true;

	_envelopeFilterCutoff = value;
}
void SynthConfiguration::SetEnvelopeFilterResonance(float value)
{
	if (_envelopeFilterResonance != value)
		_isDirty = true;

	_envelopeFilterResonance = value;
}
void SynthConfiguration::SetCompressorThreshold(float value)
{
	if (_compressorThreshold != value)
		_isDirty = true;

	_compressorThreshold = value;
}
void SynthConfiguration::SetCompressionRatio(float value)
{
	if (_compressionRatio != value)
		_isDirty = true;

	_compressionRatio = value;
}
void SynthConfiguration::SetCompressorRelaxationPeriod(float value)
{
	if (_compressorRelaxationPeriod != value)
		_isDirty = true;

	_compressorRelaxationPeriod = value;
}
void SynthConfiguration::SetCompressorAttack(float value)
{
	if (_compressorAttack != value)
		_isDirty = true;

	_compressorAttack = value;
}
void SynthConfiguration::SetCompressorRelease(float value)
{
	if (_compressorRelease != value)
		_isDirty = true;

	_compressorRelease = value;
}
void SynthConfiguration::SetDelaySeconds(float value)
{
	if (_delaySeconds != value)
		_isDirty = true;

	_delaySeconds = value;
}
void SynthConfiguration::SetDelayFeedback(bool value)
{
	if (_delayFeedback != value)
		_isDirty = true;

	_delayFeedback = value;
}
