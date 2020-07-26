﻿#include "AllPassFilter.h"

AllPassFilter::AllPassFilter(float delaySeconds, float gain, int samplingRate)
{
	int bufferSize = (int)(delaySeconds * samplingRate);

	_delayedInput = new std::queue<float>();
	_delayedOutput = new std::queue<float>();
	_gain = gain;

	// Initialize the queue
	for (int i = 0; i < bufferSize; i++)
	{
		_delayedInput->push(0);
		_delayedOutput->push(0);
	}
}
AllPassFilter::~AllPassFilter()
{
	delete _delayedInput;
	delete _delayedOutput;
}
float AllPassFilter::Apply(float sample)
{
	// https://medium.com/the-seekers-project/coding-a-basic-reverb-algorithm-part-2-an-introduction-to-audio-programming-4db79dd4e325
	//
	// y[n] = (−g·x[n]) + x[n−M] + (g·y[n−M])
	//

	// Calculate sample from front of the queue
	float result = (-1 * _gain * sample) + _delayedInput->front() + (_gain * _delayedOutput->front());

	// Queue the result and the input sample
	_delayedOutput->pop();
	_delayedInput->pop();
	_delayedOutput->push(result);
	_delayedInput->push(sample);

	return result;
}