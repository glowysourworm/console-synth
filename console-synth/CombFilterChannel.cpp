#include "CombFilterChannel.h"
#include "FilterChannelBase.h"
#include <queue>

CombFilterChannel::CombFilterChannel(float delaySeconds, float gain, bool feedback, unsigned int samplingRate)
	: FilterChannelBase(gain, samplingRate)
{
	int bufferSize = (int)(delaySeconds * samplingRate);

	_buffer = new std::queue<float>();
	_feedback = feedback;

	// Initialize the queue
	for (int i = 0; i < bufferSize; i++)
		_buffer->push(0);
}
CombFilterChannel::~CombFilterChannel()
{
	delete _buffer;
}
float CombFilterChannel::Apply(float sample, float absoluteTime)
{
	// Calculate sample from front of the queue (SAME FOR BOTH FORMS)
	float result = sample + (this->GetGain() * _buffer->front());

	// Remove used sample
	_buffer->pop();

	// Feed-forward -> store input sample
	if (!_feedback)
		_buffer->push(sample);

	// Feed-back -> store result
	else
		_buffer->push(result);

	return result;
}

bool CombFilterChannel::HasOutput(float absoluteTime) const
{
	//return _buffer->size() > 0 &&
	//	   _buffer->front() > 0 &&
	//	   this->GetGain() > 0;

	return this->GetGain() > 0;
}
