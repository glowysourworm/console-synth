#ifndef COMBFILTER_H
#define COMBFILTER_H

#include <queue>

class CombFilter
{
public:

	// Initializes a feed-back comb filter
	CombFilter(float delaySeconds, float gain, int samplingRate, bool feedbackForm = true);
	~CombFilter();

	float Apply(float sample);

private:

	std::queue<float>* _buffer;

	float _gain;
	bool _feedbackForm;
};

#endif