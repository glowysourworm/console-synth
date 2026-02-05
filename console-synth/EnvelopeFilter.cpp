#include "ButterworthFilter.h"
#include "Constant.h"
#include "Envelope.h"
#include "EnvelopeFilter.h"
#include "FilterBase.h"
#include "RandomOscillator.h"
#include "SawtoothOscillator.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "TriangleOscillator.h"
#include <cmath>
#include <exception>

EnvelopeFilter::EnvelopeFilter(float gain, 
	int samplingRate, 
	int cutoffFrequency, 
	float resonance, 
	EnvelopeFilterType filterType,
	AmplitudeOscillatorType oscillatorType,
	float oscillatorFrequency,
	const Envelope& envelope) : FilterBase(gain, samplingRate)
{
	_filter = new ButterworthFilter(samplingRate, gain);
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
	case AmplitudeOscillatorType::Sine:
		_filterOscillator = new SineOscillator(oscillatorFrequency);
		break;
	case AmplitudeOscillatorType::Square:
		_filterOscillator = new SquareOscillator(oscillatorFrequency);
		break;
	case AmplitudeOscillatorType::Triangle:
		_filterOscillator = new TriangleOscillator(oscillatorFrequency);
		break;
	case AmplitudeOscillatorType::Sawtooth:
		_filterOscillator = new SawtoothOscillator(oscillatorFrequency);
		break;
	case AmplitudeOscillatorType::Random:
		_filterOscillator = new RandomOscillator(oscillatorFrequency, 0, 1, 4);
		break;
	default:
		throw new std::exception("Unhandled Amplitude Oscillator Type:  EnvelopeFilter.cpp");
	}
}
EnvelopeFilter::~EnvelopeFilter()
{
	delete _filter;
	delete _filterEnvelope;
	delete _filterOscillator;
}
float EnvelopeFilter::ApplyConstant(float sample, float absoluteTime)
{
	return _filter->Apply(sample, absoluteTime);
}

float EnvelopeFilter::ApplyEnvelope(float sample, float absoluteTime)
{
	// Output should go from [0,1]
	float filterLevel = _filterEnvelope->GetEnvelopeLevel(absoluteTime);

	// Set Butterworth based on envelope level
	_filter->Set(filterLevel * _cutoffFrequency, _resonance);

	// Return filtered output
	return _filter->Apply(sample, absoluteTime);
}

float EnvelopeFilter::ApplyOscillator(float sample, float absoluteTime)
{
	// Oscillator Level (similar to VCO in a synth) (absolute value)
	float filterLevel = fabsf(_filterOscillator->GetSample(absoluteTime));

	// Set Butterworth based on the oscillator
	_filter->Set(filterLevel * _cutoffFrequency, _resonance);

	// Return filtered output
	return _filter->Apply(sample, absoluteTime);
}

float EnvelopeFilter::Apply(float sample, float absoluteTime)
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

void EnvelopeFilter::Engage(float absoluteTime)
{
	_filterEnvelope->Engage(absoluteTime);
}

void EnvelopeFilter::DisEngage(float absoluteTime)
{
	_filterEnvelope->DisEngage(absoluteTime);
}

bool EnvelopeFilter::HasOutput(float absoluteTime) const
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
