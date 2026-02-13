#include "Constant.h"
#include "Envelope.h"
#include "SynthConfiguration.h"
#include "SynthNoteMap.h"
#include "WindowsKeyCodes.h"
#include <AirwinRegistry.h>
#include <atomic>
#include <thread>

SynthConfiguration::SynthConfiguration()
{
	_airwinEffectRegistry = new AirwinRegistry();
	_keyMap = new SynthNoteMap();

	// LOAD AIRWIN PLUGINS! (This may take a couple seconds)
	_airwinEffectRegistry->Load(0);
	
	_waitFlag = false;							// std::atomic
	_isDirty = false;

	_oscillatorType = OscillatorType::Sine;

	_midiLow = MIDI_PIANO_LOW_NUMBER;
	_midiHigh = MIDI_PIANO_HIGH_NUMBER;

	_hasDelay = false;
	_hasCompressor = false;
	_hasEnvelopeFilter = false;
	_hasReverb = false;

	_noteEnvelope = new Envelope();
	_envelopeFilter = new Envelope();

	_envelopeFilterCutoff = 5000;
	_envelopeFilterResonance = 1;
	_envelopeFilterType = EnvelopeFilterType::Constant;
	_envelopeFilterOscillatorType = OscillatorType::Sine;
	_envelopeFilterOscillatorFrequency = 1.0;

	_compressorThreshold = 0.5;
	_compressionRatio = 1;
	_compressorRelaxationPeriod = 1;
	_compressorAttack = 0.2;
	_compressorRelease = 0.3;
	_compressorGain = 1.0;

	_delaySeconds = 0.5;
	_delayFeedback = false;
	_delayGain = 0.6;
	_delayWetDry = 0.8f;

	_reverbDelaySeconds = 0.2;
	_reverbGain = 0.5;
	_reverbWetDry = 0.5f;

	_leftRight = 0.5f;
	_gain = 1.0f;
}
SynthConfiguration::SynthConfiguration(const SynthConfiguration& copy)
{
	if (_keyMap != nullptr)
		delete _keyMap;

	if (_noteEnvelope != nullptr)
		delete _noteEnvelope;

	if (_envelopeFilter != nullptr)
		delete _envelopeFilter;

	_airwinEffectRegistry = copy.GetEffectRegistry();

	_midiLow = copy.GetMidiLow();
	_midiHigh = copy.GetMidiHigh();

	_oscillatorType = copy.GetOscillatorType();

	_hasReverb = copy.GetHasReverb();
	_hasDelay = copy.GetHasDelay();
	_hasCompressor = copy.GetHasCompressor();
	_hasEnvelopeFilter = copy.GetHasEnvelopeFilter();

	_keyMap = new SynthNoteMap(copy.GetNoteMap());
	_noteEnvelope = new Envelope(copy.GetNoteEnvelope());
	_envelopeFilter = new Envelope(copy.GetEnvelopeFilter());

	_envelopeFilterCutoff = copy.GetEnvelopeFilterCutoff();
	_envelopeFilterResonance = copy.GetEnvelopeFilterResonance();
	_envelopeFilterOscillatorFrequency = copy.GetEnvelopeFilterOscillatorFrequency();
	_envelopeFilterType = copy.GetEnvelopeFilterType();
	_envelopeFilterOscillatorType = copy.GetEnvelopeFilterOscillatorType();

	_compressorThreshold = copy.GetCompressorThreshold();
	_compressionRatio = copy.GetCompressionRatio();
	_compressorRelaxationPeriod = copy.GetCompressorRelaxationPeriod();
	_compressorAttack = copy.GetCompressorAttack();
	_compressorRelease = copy.GetCompressorRelease();
	_compressorGain = copy.GetCompressorGain();

	_reverbDelaySeconds = copy.GetReverbDelaySeconds();
	_reverbGain = copy.GetReverbGain();
	_reverbWetDry = copy.GetReverbWetDry();

	_delaySeconds = copy.GetDelaySeconds();
	_delayFeedback = copy.GetDelayFeedback();
	_delayWetDry = copy.GetDelayWetDry();

	_leftRight = copy.GetOutputLeftRight();
	_gain = copy.GetOutputGain();
}
SynthConfiguration::~SynthConfiguration()
{
	if (_airwinEffectRegistry != nullptr)
		delete _airwinEffectRegistry;

	if (_keyMap != nullptr)
		delete _keyMap;

	if (_noteEnvelope != nullptr)
		delete _noteEnvelope;

	if (_envelopeFilter != nullptr)
		delete _envelopeFilter;
}

bool SynthConfiguration::IsWaiting() const
{
	return _waitFlag;	// std::atomic
}

bool SynthConfiguration::IsDirty() const
{
	return _isDirty;
}

bool SynthConfiguration::SetWait(bool desiredValue)
{
	std::thread::id threadId = std::this_thread::get_id();

	// Waiting on another thread
	if (_waitFlag && _threadId != threadId)
		return false;

	else
	{
		// Set thread's desired value
		_waitFlag = desiredValue;					// std::atomic

		// "Locking" for this thread
		if (desiredValue)
			_threadId = threadId;
		
		return true;
	}
}

void SynthConfiguration::ClearDirty()
{
	_isDirty = false;
}

OscillatorType SynthConfiguration::GetOscillatorType() const
{
	return _oscillatorType;
}
bool SynthConfiguration::GetHasReverb() const
{
	return _hasReverb;
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
SynthNoteMap SynthConfiguration::GetNoteMap() const
{
	return *_keyMap;
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
EnvelopeFilterType SynthConfiguration::GetEnvelopeFilterType() const
{
	return _envelopeFilterType;
}
OscillatorType SynthConfiguration::GetEnvelopeFilterOscillatorType() const
{
	return _envelopeFilterOscillatorType;
}
float SynthConfiguration::GetEnvelopeFilterOscillatorFrequency() const
{
	return _envelopeFilterOscillatorFrequency;
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
float SynthConfiguration::GetCompressorGain() const
{
	return _compressorGain;
}
float SynthConfiguration::GetReverbDelaySeconds() const
{
	return _reverbDelaySeconds;
}
float SynthConfiguration::GetReverbGain() const
{
	return _reverbGain;
}
float SynthConfiguration::GetReverbWetDry() const
{
	return _reverbWetDry;
}
float SynthConfiguration::GetDelaySeconds() const
{
	return _delaySeconds;
}
float SynthConfiguration::GetDelayGain() const
{
	return _delayGain;
}
bool SynthConfiguration::GetDelayFeedback() const
{
	return _delayFeedback;
}
float SynthConfiguration::GetDelayWetDry() const
{
	return _delayWetDry;
}
float SynthConfiguration::GetOutputLeftRight() const
{
	return _leftRight;
}
float SynthConfiguration::GetOutputGain() const
{
	return _gain;
}
void SynthConfiguration::IterateKeymap(SynthNoteMap::KeymapIterationCallback callback) const
{
	_keyMap->Iterate(callback);
}
int SynthConfiguration::GetMidiLow() const
{
	return _midiLow;
}
int SynthConfiguration::GetMidiHigh() const
{
	return _midiHigh;
}

bool SynthConfiguration::HasMidiNote(WindowsKeyCodes keyCode) const
{
	return _keyMap->HasMidiNote(keyCode);
}

int SynthConfiguration::GetMidiNote(WindowsKeyCodes keyCode) const
{
	return _keyMap->GetMidiNote(keyCode);
}

WindowsKeyCodes SynthConfiguration::GetKeyCode(int midiNote) const
{
	return _keyMap->GetKeyCode(midiNote);
}

void SynthConfiguration::SetOscillatorType(OscillatorType value)
{
	if (_oscillatorType != value)
		_isDirty = true;

	_oscillatorType = value;
}
void SynthConfiguration::SetEnvelopeFilterType(EnvelopeFilterType value)
{
	if (_envelopeFilterType != value)
		_isDirty = true;

	_envelopeFilterType = value;
}
void SynthConfiguration::SetEnvelopeFilterOscillatorType(OscillatorType value)
{
	if (_envelopeFilterOscillatorType != value)
		_isDirty = true;

	_envelopeFilterOscillatorType = value;
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
void SynthConfiguration::SetMidiNote(WindowsKeyCodes keyCode, int midiNote)
{
	_keyMap->Add(keyCode, midiNote);
}
void SynthConfiguration::SetHasReverb(bool value)
{
	if (_hasReverb != value)
		_isDirty = true;

	_hasReverb = value;
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
void SynthConfiguration::SetEnvelopeFilterOscillatorFrequency(float value)
{
	if (_envelopeFilterOscillatorFrequency != value)
		_isDirty = true;

	_envelopeFilterOscillatorFrequency = value;
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
void SynthConfiguration::SetCompressorGain(float value)
{
	if (_compressorGain != value)
		_isDirty = true;

	_compressorGain = value;
}
void SynthConfiguration::SetReverbSeconds(float value)
{
	if (_reverbDelaySeconds != value)
		_isDirty = true;

	_reverbDelaySeconds = value;
}
void SynthConfiguration::SetReverbGain(float value)
{
	if (_reverbGain != value)
		_isDirty = true;

	_reverbGain = value;
}
void SynthConfiguration::SetReverbWetDry(float value)
{
	if (_reverbWetDry != value)
		_isDirty = true;

	_reverbWetDry = value;
}
void SynthConfiguration::SetDelaySeconds(float value)
{
	if (_delaySeconds != value)
		_isDirty = true;

	_delaySeconds = value;
}
void SynthConfiguration::SetDelayGain(float value)
{
	if (_delayGain != value)
		_isDirty = true;

	_delayGain = value;
}
void SynthConfiguration::SetDelayFeedback(bool value)
{
	if (_delayFeedback != value)
		_isDirty = true;

	_delayFeedback = value;
}
void SynthConfiguration::SetDelayWetDry(float value)
{
	if (_delayWetDry != value)
		_isDirty = true;

	_delayWetDry = value;
}
void SynthConfiguration::SetOutputLeftRight(float value)
{
	if (_leftRight != value)
		_isDirty = true;

	_leftRight = value;
}
void SynthConfiguration::SetOutputGain(float value)
{
	if (_gain != value)
		_isDirty = true;

	_gain = value;
}

AirwinRegistry* SynthConfiguration::GetEffectRegistry() const
{
	return _airwinEffectRegistry;
}
