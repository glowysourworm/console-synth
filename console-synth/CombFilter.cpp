#include "CombFilter.h"

CombFilter::CombFilter(float delaySeconds, float gain, int samplingRate)
{
	int bufferSize = (int)(delaySeconds * samplingRate);

	_delayedOutput = new std::queue<float>();
	_gain = gain;
	
	// Initialize the queue
	for (int i = 0; i < bufferSize; i++)
		_delayedOutput->push(0);
}
CombFilter::~CombFilter()
{
	delete _delayedOutput;
}
float CombFilter::Apply(float sample)
{
	// Calculate sample from front of the queue
	float result = sample + (_gain * _delayedOutput->front());

	// Queue the result
	_delayedOutput->pop();
	_delayedOutput->push(result);

	return result;
}
