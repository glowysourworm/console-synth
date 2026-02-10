#pragma once
#include "ButterworthFilterChannel.h"
#include "Constant.h"
#include "Envelope.h"
#include "FilterChannelBase.h"
#include "OscillatorBase.h"
#include "SynthConfiguration.h"

class EnvelopeFilterChannel : public FilterChannelBase
{
public:

	EnvelopeFilterChannel(float gain,							
		int cutoffFrequency,
		float resonance,
		EnvelopeFilterType filterType,
		OscillatorType oscillatorType,
		float oscillatorFrequency,
		const Envelope& envelope, 
		unsigned int samplingRate);
	~EnvelopeFilterChannel();

	float Apply(float inputSample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;
	void SetConfiguration(const SynthConfiguration* configuration) override;

	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);

private:

	void ConstructOscillator();

	float ApplyConstant(float sample, float absoluteTime);
	float ApplyEnvelope(float sample, float absoluteTime);
	float ApplyOscillator(float sample, float absoluteTime);

private:

	int _cutoffFrequency;
	float _resonance;

	ButterworthFilterChannel* _filter;
	Envelope* _filterEnvelope;
	OscillatorBase* _filterOscillator;

	float _oscillatorFrequency;
	EnvelopeFilterType _filterType;
	OscillatorType _oscillatorType;
};

