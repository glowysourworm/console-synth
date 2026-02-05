#include "Compressor.h"
#include "Constant.h"
#include "FilterBase.h"
#include <cmath>
#include <queue>

Compressor::Compressor(float gain,
	int samplingRate,
	float threshold,
	float compressionRatio,
	float relaxationPeriod,
	float attack,
	float release) : FilterBase(gain, samplingRate)
{
	// Compression parameters
	_threshold = threshold;
	_compressionRatio = compressionRatio;
	_relaxationValue = 0;
	_relaxationBuffer = new std::queue<float>();
	_relaxationBufferLength = SAMPLING_RATE * relaxationPeriod;
	_attack = attack;
	_release = release;
}

Compressor::~Compressor()
{
	delete _relaxationBuffer;
}

float Compressor::Apply(float sample, float absoluteTime)
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

bool Compressor::HasOutput(float absoluteTime) const
{
	return true;
}
