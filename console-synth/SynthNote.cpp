#include "Constant.h"
#include "Envelope.h"
#include "EnvelopeFilter.h"
#include "RandomOscillator.h"
#include "SawtoothOscillator.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"
#include "TriangleOscillator.h"
#include <cmath>
#include <exception>

SynthNote::SynthNote(int midiNumber, const SynthConfiguration& configuration)
{
	_midiNumber = midiNumber;
	_envelope = new Envelope(configuration.GetNoteEnvelope());
	_envelopeFilter = new EnvelopeFilter(1.0, 
		SAMPLING_RATE,
		configuration.GetEnvelopeFilterCutoff(),
		configuration.GetEnvelopeFilterResonance(),
		configuration.GetEnvelopeFilterType(),
		configuration.GetEnvelopeFilterOscillatorType(),
		configuration.GetEnvelopeFilterOscillatorFrequency(),
		configuration.GetEnvelopeFilter());

	_envelopeFilterEnabled = configuration.GetHasEnvelopeFilter();

	// Initialize Oscillator
	switch (configuration.GetOscillatorType())
	{
	case AmplitudeOscillatorType::Sine:
		_oscillator = new SineOscillator(this->GetFrequency());
		break;
	case AmplitudeOscillatorType::Random:
		_oscillator = new RandomOscillator(this->GetFrequency(), SIGNAL_LOW, SIGNAL_HIGH, 4);
		break;
	case AmplitudeOscillatorType::Square:
		_oscillator = new SquareOscillator(this->GetFrequency());
		break;
	case AmplitudeOscillatorType::Triangle:
		_oscillator = new TriangleOscillator(this->GetFrequency());
		break;
	case AmplitudeOscillatorType::Sawtooth:
		_oscillator = new SawtoothOscillator(this->GetFrequency());
		break;
	default:
		throw std::exception("Unhandled Oscillator Type:  SynthNote.cpp");
	}
}

SynthNote::~SynthNote()
{
	delete _envelope;
}

int SynthNote::GetMidiNumber() const
{
	return _midiNumber;
}

float SynthNote::GetSample(float absoluteTime) const
{
	// BASE TONE:  Includes note envelope
	float output = _envelope->GetEnvelopeLevel(absoluteTime) * _oscillator->GetSample(absoluteTime);

	// FILTER SWEEP: Check envelope filter for output
	if (_envelopeFilterEnabled && _envelopeFilter->HasOutput(absoluteTime))
	{
		output = _envelopeFilter->Apply(output, absoluteTime);
	}

	return output;
}

float SynthNote::GetFrequency() const
{
	return 440 * powf(2, ((_midiNumber - 69.0) / 12.0));
}

bool SynthNote::IsEngaged()
{
	return _envelope->IsEngaged();
}

bool SynthNote::HasOutput(float absoluteTime)
{
	return _envelope->HasOutput(absoluteTime);
}

void SynthNote::Engage(float absoluteTime)
{
	_envelope->Engage(absoluteTime);
	_envelopeFilter->Engage(absoluteTime);
}

void SynthNote::DisEngage(float absoluteTime)
{
	_envelope->DisEngage(absoluteTime);
	_envelopeFilter->DisEngage(absoluteTime);
}