#include "CompressorChannel.h"
#include "FilterChannelBase.h"
#include <cmath>
#include <queue>

CompressorChannel::CompressorChannel(float gain,
	unsigned int samplingRate,
	float threshold,
	float compressionRatio,
	float relaxationPeriod,
	float attack,
	float release) : FilterChannelBase(gain, samplingRate)
{
	// Compression parameters
	_threshold = threshold;
	_compressionRatio = compressionRatio;
	_relaxationValue = 0;
	_relaxationBuffer = new std::queue<float>();
	_relaxationBufferLength = samplingRate * relaxationPeriod;
	_attack = attack;
	_release = release;
}

CompressorChannel::~CompressorChannel()
{
	delete _relaxationBuffer;
}

float CompressorChannel::Apply(float sample, float absoluteTime)
{
	float slope = 1 / _compressionRatio;
	float result = 0;

	// Hard knee compression
	if (fabsf(sample) > _threshold)
	{
		result = this->GetGain() * (slope * sample) + (_threshold * (1 - slope));
	}

	// BYPASS
	else
	{
		result = this->GetGain() * sample;
	}

	return result;
}

bool CompressorChannel::HasOutput(float absoluteTime) const
{
	return true;
}
