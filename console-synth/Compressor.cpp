#include "Compressor.h"
#include "Constant.h"
#include <cmath>
#include <queue>

Compressor::Compressor(float gain, float threshold, float compressionRatio, float relaxationPeriod, float attack, float release, bool cutoffLineLimit)
{
	// Compression parameters
	_gain = gain;
	_threshold = threshold;
	_compressionRatio = compressionRatio;
	_cutoff = cutoffLineLimit;
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

float Compressor::Apply(float absoluteTime, float sample)
{
	float slope = 1 / _compressionRatio;
	float result = 0;

	// Hard knee compression
	if (fabsf(sample) > _threshold)
	{
		if (sample > 0)
		{
			// Hard limit at 1
			result = _gain * fminf((slope * sample) + (_threshold * (1 - slope)), 1.0f);
		}
		else
		{
			// Hard limit at -1
			result = _gain * fmaxf((slope * sample) + (_threshold * (slope - 1)), -1.0f);
		}
	}

	// BYPASS
	else
	{
		result = _gain * sample;
	}

	// Line Limit
	if (_cutoff && result > 1)
		result = 1;

	if (_cutoff && result < -1)
		result = -1;

	return result;
}
