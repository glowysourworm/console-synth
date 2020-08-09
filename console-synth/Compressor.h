#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <math.h>
#include <queue>

#include "Constant.h"

class Compressor
{
public:

	// Compression ratio is a value between [1, Infinity]
	Compressor(float gain, float threshold, float compressionRatio, float relaxationPeriod, float attack, float release);
	~Compressor();

	float Apply(float absoluteTime, float sample);

private:


	float _gain;
	float _threshold;
	float _compressionRatio;
	float _attack;
	float _release;

	// Beginning of moving average window
	std::queue<float>* _relaxationBuffer;
	int _relaxationBufferLength;
	float _relaxationValue;
};

#endif

