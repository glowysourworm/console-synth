#include "ButterworthFilter.h"
#include "Envelope.h"
#include "EnvelopeFilter.h"
#include "FilterBase.h"

EnvelopeFilter::EnvelopeFilter(float gain, int samplingRate, int cutoffFrequency, float resonance, const Envelope& envelope) : FilterBase(gain, samplingRate)
{
	_filter = new ButterworthFilter(samplingRate, gain);
	_filterEnvelope = new Envelope(envelope);

	_cutoffFrequency = cutoffFrequency;
	_resonance = resonance;
}
EnvelopeFilter::~EnvelopeFilter()
{
	delete _filter;
	delete _filterEnvelope;
}

float EnvelopeFilter::Apply(float sample, float absoluteTime)
{
	// Output should go from [0,1]
	float filterLevel = _filterEnvelope->GetEnvelopeLevel(absoluteTime);

	// Set Butterworth based on envelope level
	_filter->Set(filterLevel * _cutoffFrequency, _resonance);

	// Return filtered output
	return _filter->Apply(sample, absoluteTime);
}

void EnvelopeFilter::Engage(float absoluteTime)
{
	_filterEnvelope->Engage(absoluteTime);
}

void EnvelopeFilter::DisEngage(float absoluteTime)
{
	_filterEnvelope->DisEngage(absoluteTime);
}

bool EnvelopeFilter::HasOutput(float absoluteTime)
{
	return _filterEnvelope->HasOutput(absoluteTime);
}
