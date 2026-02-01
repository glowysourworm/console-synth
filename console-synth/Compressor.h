#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <queue>

class Compressor
{
public:

	// Compression ratio is a value between [1, Infinity]
	Compressor(float gain, float threshold, float compressionRatio, float relaxationPeriod, float attack, float release, bool cutoffLineLimit);
	~Compressor();

	float Apply(float absoluteTime, float sample);

private:


	float _gain;
	float _threshold;
	float _compressionRatio;
	float _attack;
	float _release;
	bool _cutoff;

	// Beginning of moving average window
	std::queue<float>* _relaxationBuffer;
	int _relaxationBufferLength;
	float _relaxationValue;
};

#endif

