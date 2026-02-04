#pragma once
#include "ButterworthFilter.h"
#include "Envelope.h"
#include "FilterBase.h"

class EnvelopeFilter : public FilterBase
{
public:

	EnvelopeFilter(float gain, int samplingRate, int cutoffFrequency, float resonance, const Envelope& envelope);
	~EnvelopeFilter();

	float Apply(float sample, float absoluteTime) override;

	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);
	bool HasOutput(float absoluteTime);

private:

	int _cutoffFrequency;
	float _resonance;

	ButterworthFilter* _filter;
	Envelope* _filterEnvelope;
};

