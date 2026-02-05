#include "CombFilter.h"
#include "FilterBase.h"
#include <queue>

CombFilter::CombFilter(float delaySeconds, float gain, int samplingRate, bool feedbackForm) : FilterBase(gain, samplingRate)
{
	int bufferSize = (int)(delaySeconds * samplingRate);

	_buffer = new std::queue<float>();
	_feedbackForm = feedbackForm;

	// Initialize the queue
	for (int i = 0; i < bufferSize; i++)
		_buffer->push(0);
}
CombFilter::~CombFilter()
{
	delete _buffer;
}
float CombFilter::Apply(float sample, float absoluteTime)
{
	// Calculate sample from front of the queue (SAME FOR BOTH FORMS)
	float result = sample + (this->GetGain() * _buffer->front());

	// Remove used sample
	_buffer->pop();

	// Feed-forward -> store input sample
	if (!_feedbackForm)
		_buffer->push(sample);

	// Feed-back -> store result
	else
		_buffer->push(result);

	return result;
}

bool CombFilter::HasOutput(float absoluteTime) const
{
	return _buffer->size() > 0;
}
