#pragma once

#include "FilterChannelBase.h"
#include "SynthConfiguration.h"
#include <queue>

class CompressorChannel : public FilterChannelBase
{
public:

	// Compression ratio is a value between [1, Infinity]
	CompressorChannel(float gain, unsigned int samplingRate, float threshold, float compressionRatio, float relaxationPeriod, float attack, float release);
	~CompressorChannel();

	float Apply(float inputSample, float absoluteTime) override;
	bool HasOutput(float absoluteTime) const override;
	void SetConfiguration(const SynthConfiguration* configuration) override;

private:


	float _threshold;
	float _compressionRatio;
	float _attack;
	float _release;

	// Beginning of moving average window
	std::queue<float>* _relaxationBuffer;
	int _relaxationBufferLength;
	float _relaxationValue;
};