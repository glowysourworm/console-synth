#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "FilterBase.h"
#include <queue>

class Compressor : public FilterBase
{
public:

	// Compression ratio is a value between [1, Infinity]
	Compressor(float gain, int samplingRate, float threshold, float compressionRatio, float relaxationPeriod, float attack, float release);
	~Compressor();

	float Apply(float sample, float absoluteTime) override;

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

#endif

