#include "ButterworthFilterChannel.h"
#include "Constant.h"
#include "Envelope.h"
#include "EnvelopeFilterChannel.h"
#include "FilterChannelBase.h"
#include "RandomOscillator.h"
#include "SawtoothOscillator.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "TriangleOscillator.h"
#include <cmath>
#include <exception>

EnvelopeFilterChannel::EnvelopeFilterChannel(float gain,
	int cutoffFrequency,
	float resonance,
	EnvelopeFilterType filterType,
	OscillatorType oscillatorType,
	float oscillatorFrequency,
	const Envelope& envelope,
	unsigned int samplingRate) : FilterChannelBase(gain, samplingRate)
{
	_filter = new ButterworthFilterChannel(samplingRate, gain);
	_filterEnvelope = new Envelope(envelope);

	_oscillatorFrequency = oscillatorFrequency;
	_filterType = filterType;
	_oscillatorType = oscillatorType;

	_cutoffFrequency = cutoffFrequency;
	_resonance = resonance;

	// Initialize Butterworth Filter (this may not be reset in real time. depends on usage.)
	_filter->Set(_cutoffFrequency, _resonance);

	switch (oscillatorType)
	{
	case OscillatorType::Sine:
		_filterOscillator = new SineOscillator(oscillatorFrequency);
		break;
	case OscillatorType::Square:
		_filterOscillator = new SquareOscillator(oscillatorFrequency);
		break;
	case OscillatorType::Triangle:
		_filterOscillator = new TriangleOscillator(oscillatorFrequency);
		break;
	case OscillatorType::Sawtooth:
		_filterOscillator = new SawtoothOscillator(oscillatorFrequency);
		break;
	case OscillatorType::Random:
		_filterOscillator = new RandomOscillator(oscillatorFrequency, 4);
		break;
	default:
		throw new std::exception("Unhandled Amplitude Oscillator Type:  EnvelopeFilter.cpp");
	}
}
EnvelopeFilterChannel::~EnvelopeFilterChannel()
{
	delete _filter;
	delete _filterEnvelope;
	delete _filterOscillator;
}
float EnvelopeFilterChannel::ApplyConstant(float sample, float absoluteTime)
{
	return _filter->Apply(sample, absoluteTime);
}

float EnvelopeFilterChannel::ApplyEnvelope(float sample, float absoluteTime)
{
	// Output should go from [0,1]
	float filterLevel = _filterEnvelope->GetEnvelopeLevel(absoluteTime);

	// Set Butterworth based on envelope level
	_filter->Set(filterLevel * _cutoffFrequency, _resonance);

	// Return filtered output
	return _filter->Apply(sample, absoluteTime);
}

float EnvelopeFilterChannel::ApplyOscillator(float sample, float absoluteTime)
{
	// Oscillator Level (similar to VCO in a synth) (absolute value)
	float filterLevel = fabsf(_filterOscillator->GetMonoSample(absoluteTime));

	// Set Butterworth based on the oscillator
	_filter->Set(filterLevel * _cutoffFrequency, _resonance);

	// Return filtered output
	return _filter->Apply(sample, absoluteTime);
}

float EnvelopeFilterChannel::Apply(float sample, float absoluteTime)
{
	switch (_filterType)
	{
	case EnvelopeFilterType::Constant:
		return ApplyConstant(sample, absoluteTime);

	case EnvelopeFilterType::Oscillator:
		return ApplyOscillator(sample, absoluteTime);

	case EnvelopeFilterType::EnvelopeSweep:
		return ApplyEnvelope(sample, absoluteTime);
	default:
		throw new std::exception("Unhandled Envelope Filter Type:  EnvelopeFilter.cpp");
	}
}

void EnvelopeFilterChannel::Engage(float absoluteTime)
{
	_filterEnvelope->Engage(absoluteTime);
}

void EnvelopeFilterChannel::DisEngage(float absoluteTime)
{
	_filterEnvelope->DisEngage(absoluteTime);
}

bool EnvelopeFilterChannel::HasOutput(float absoluteTime) const
{
	switch (_filterType)
	{
	case EnvelopeFilterType::Constant:
		return true;
	case EnvelopeFilterType::Oscillator:
		return true;
	case EnvelopeFilterType::EnvelopeSweep:
		return _filterEnvelope->HasOutput(absoluteTime);
	default:
		throw new std::exception("Unhandled Envelope Fitler Type:  EnvelopeFilter.cpp");
	}	
}
