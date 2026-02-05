#pragma once
#include "AmplitudeOscillator.h"
#include "ButterworthFilter.h"
#include "Constant.h"
#include "Envelope.h"
#include "FilterBase.h"

class EnvelopeFilter : public FilterBase
{
public:

	EnvelopeFilter(float gain,
					int samplingRate,
					int cutoffFrequency,
					float resonance,
					EnvelopeFilterType filterType,
					AmplitudeOscillatorType oscillatorType,
					float oscillatorFrequency,
					const Envelope& envelope);
	~EnvelopeFilter();

	float Apply(float sample, float absoluteTime) override;

	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);
	bool HasOutput(float absoluteTime) const override;

private:

	float ApplyConstant(float sample, float absoluteTime);
	float ApplyEnvelope(float sample, float absoluteTime);
	float ApplyOscillator(float sample, float absoluteTime);

private:

	int _cutoffFrequency;
	float _resonance;

	ButterworthFilter* _filter;
	Envelope* _filterEnvelope;
	AmplitudeOscillator* _filterOscillator;

	float _oscillatorFrequency;
	EnvelopeFilterType _filterType;
	AmplitudeOscillatorType _oscillatorType;
};

