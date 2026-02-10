#ifndef AMPLITUDEOSCILLATOR_H
#define AMPLITUDEOSCILLATOR_H

#include "Constant.h"
#include "PlaybackFrame.h"
#include "SignalBase.h"
#include <cmath>

class OscillatorBase : public SignalBase
{
public:
	OscillatorBase(float frequency);
	virtual ~OscillatorBase();

	void Set(float frequency);

	void GetSample(PlaybackFrame* frame, float absoluteTime)
	{
		float sample = this->GetMonoSample(absoluteTime);

		// Mono (L / R)
		frame->SetFrame(sample, sample);
	}
	virtual float GetMonoSample(float absoluteTime) = 0;

	float GetFrequency() const;
	float GetPeriod() const;
	virtual OscillatorType GetType() const = 0;

private:

	float _frequency;
	float _period;
};

#endif