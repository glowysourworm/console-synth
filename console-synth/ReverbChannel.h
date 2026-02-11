#pragma once
#include "AllPassFilterChannel.h"
#include "ButterworthFilterChannel.h"
#include "CombFilterChannel.h"
#include "FilterChannelBase.h"
#include "SynthConfiguration.h"

// Implements Schroeder Reverb
//
class ReverbChannel : public FilterChannelBase
{
public:
	ReverbChannel(float delaySeconds, float gain, int samplingRate);
	~ReverbChannel();

	float Apply(float sample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;
	void SetConfiguration(const SynthConfiguration* configuration) override;

private:
	unsigned int _samplingRate;
	CombFilterChannel** _combFilters;
	AllPassFilterChannel** _allPassFilters;
	ButterworthFilterChannel* _lowPassFilter;
};