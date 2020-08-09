#include "Compressor.h"

Compressor::Compressor(float gain, float threshold, float compressionRatio, float relaxationPeriod, float attack, float release)
{
	// Compression parameters
	_gain = gain;
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

float Compressor::Apply(float absoluteTime, float sample)
{
	float slope = 1 / _compressionRatio;

	// Hard knee compression
	if (fabsf(sample) > _threshold)
	{
		if (sample > 0)
		{
			// Hard limit at 1
			return _gain * fminf((slope * sample) + (_threshold * (1 - slope)), 1.0f);
		}
		else
		{
			// Hard limit at -1
			return _gain * fmaxf((slope * sample) + (_threshold * (slope - 1)), -1.0f);
		}


	}

	// BYPASS
	else
	{
		return _gain * sample;
	}
}
