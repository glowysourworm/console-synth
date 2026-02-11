#include "AllPassFilterChannel.h"
#include "FilterChannelBase.h"
#include "SynthConfiguration.h"
#include <queue>

AllPassFilterChannel::AllPassFilterChannel(float delaySeconds, float gain, unsigned int samplingRate) : FilterChannelBase(gain, samplingRate)
{
	_bufferSize = (int)(delaySeconds * samplingRate);

	_delayedInput = new std::queue<float>();
	_delayedOutput = new std::queue<float>();
}
AllPassFilterChannel::~AllPassFilterChannel()
{
	delete _delayedInput;
	delete _delayedOutput;
}

void AllPassFilterChannel::Set(float delaySeconds, float gain, unsigned int samplingRate)
{
	delete _delayedInput;
	delete _delayedOutput;

	_bufferSize = (int)(delaySeconds * samplingRate);

	_delayedInput = new std::queue<float>();
	_delayedOutput = new std::queue<float>();
}

float AllPassFilterChannel::Apply(float sample, float absoluteTime)
{
	// https://medium.com/the-seekers-project/coding-a-basic-reverb-algorithm-part-2-an-introduction-to-audio-programming-4db79dd4e325
	//
	// y[n] = (−g·x[n]) + x[n−M] + (g·y[n−M])
	//

	// Buffering:  Enqueue samples until you've reached capacity
	if (_delayedInput->size() < _bufferSize)
	{
		_delayedInput->push(sample);
	}

	// Applying:  Start circulating the queue back into the output stream; and filling the output buffer
	else
	{
		// Safe get from the output queue (once you've reached capacity)
		float outputSample = _delayedOutput->size() == _bufferSize ? _delayedOutput->front() : 0;

		// Calculate sample from front of the queue(s)
		float result = (-1 * this->GetGain() * sample) + _delayedInput->front() + (this->GetGain() * outputSample);

		// Queue the result and the input sample
		if (_delayedOutput->size() == _bufferSize)
			_delayedOutput->pop();

		_delayedInput->pop();
		_delayedOutput->push(result);
		_delayedInput->push(sample);

		return result;
	}

	return 0.0f;
}

bool AllPassFilterChannel::HasOutput(float absoluteTime) const
{
	return _delayedOutput->size() > 0 &&
		_delayedOutput->front() > 0 &&
		this->GetGain() > 0;
}

void AllPassFilterChannel::SetConfiguration(const SynthConfiguration* configuration)
{
}
