#ifndef REVERB_H
#define REVERB_H

#include "AllPassFilterChannel.h"
#include "ButterworthFilterChannel.h"
#include "CombFilterChannel.h"
#include "FilterChannelBase.h"

// Implements Schroeder Reverb
//
class ReverbChannel : public FilterChannelBase
{
public:
	ReverbChannel(float delaySeconds, float gain, int samplingRate);
	~ReverbChannel();

	float Apply(float sample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;

private:
	CombFilterChannel** _combFilters;
	AllPassFilterChannel** _allPassFilters;
	ButterworthFilterChannel* _lowPassFilter;
};

#endif

