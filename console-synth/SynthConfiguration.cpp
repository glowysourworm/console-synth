#include "Constant.h"
#include "Envelope.h"
#include "SynthConfiguration.h"

SynthConfiguration::SynthConfiguration()
{
	_oscillatorType = AmplitudeOscillatorType::Sine;

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

	_oscillatorType = copy.GetOscillatorType();

	_hasDelay = copy.GetHasDelay();
	_hasCompressor = copy.GetHasCompressor();
	_hasEnvelopeFilter = copy.GetHasEnvelopeFilter();

	_noteEnvelope = new Envelope(copy.GetNoteEnvelope());
	_envelopeFilter = new Envelope(copy.GetEnvelopeFilter());
}
SynthConfiguration::~SynthConfiguration()
{
	if (_noteEnvelope != nullptr)
		delete _noteEnvelope;

	if (_envelopeFilter != nullptr)
		delete _envelopeFilter;
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

void SynthConfiguration::SetOscillatorType(AmplitudeOscillatorType value)
{
	_oscillatorType = value;
}
void SynthConfiguration::SetHasDelay(bool value)
{
	_hasDelay = value;
}
void SynthConfiguration::SetHasCompressor(bool value)
{
	_hasCompressor = value;
}
void SynthConfiguration::SetHasEnvelopeFilter(bool value)
{
	_hasEnvelopeFilter = value;
}

void SynthConfiguration::SetNoteEnvelope(const Envelope& value)
{
	if (_noteEnvelope != nullptr)
		delete _noteEnvelope;

	_noteEnvelope = new Envelope(value);
}
void SynthConfiguration::SetEnvelopeFilter(const Envelope& value)
{
	if (_envelopeFilter != nullptr)
		delete _envelopeFilter;

	_envelopeFilter = new Envelope(value);
}
void SynthConfiguration::SetEnvelopeFilterCutoff(int value)
{
	_envelopeFilterCutoff = value;
}
void SynthConfiguration::SetEnvelopeFilterResonance(float value)
{
	_envelopeFilterResonance = value;
}
void SynthConfiguration::SetCompressorThreshold(float value)
{
	_compressorThreshold = value;
}
void SynthConfiguration::SetCompressionRatio(float value)
{
	_compressionRatio = value;
}
void SynthConfiguration::SetCompressorRelaxationPeriod(float value)
{
	_compressorRelaxationPeriod = value;
}
void SynthConfiguration::SetCompressorAttack(float value)
{
	_compressorAttack = value;
}
void SynthConfiguration::SetCompressorRelease(float value)
{
	_compressorRelease = value;
}
void SynthConfiguration::SetDelaySeconds(float value)
{
	_delaySeconds = value;
}
bool SynthConfiguration::GetDelayFeedback() const
{
	return _delayFeedback;
}
void SynthConfiguration::SetDelayFeedback(bool value)
{
	_delayFeedback = value;
}