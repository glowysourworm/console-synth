#ifndef AMPLITUDEOSCILLATOR_H
#define AMPLITUDEOSCILLATOR_H

#include "PlaybackFrame.h"
#include "SignalBase.h"
#include <cmath>

class OscillatorBase : public SignalBase
{
public:
	OscillatorBase(float frequency);
	virtual ~OscillatorBase();

	void MixSample(PlaybackFrame* frame, float absoluteTime)
	{
		float sample = this->GetMonoSample(absoluteTime);

		// Mono
		for (int index = 0; index < frame->GetChannelCount(); index++)
		{
			frame->SetSample(index, sample + frame->GetSample(index));
		}
		
	}
	void GetSample(PlaybackFrame* frame, float absoluteTime)
	{
		float sample = this->GetMonoSample(absoluteTime);

		// Mono (L / R)
		frame->SetFrame(sample, sample);
	}
	virtual float GetMonoSample(float absoluteTime) = 0;

	float GetFrequency() const;
	float GetPeriod() const;

private:

	float _frequency;
	float _period;
};

#endif