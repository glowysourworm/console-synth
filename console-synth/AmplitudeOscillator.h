#ifndef AMPLITUDEOSCILLATOR_H
#define AMPLITUDEOSCILLATOR_H

class AmplitudeOscillator
{
public:
	AmplitudeOscillator() {};
	AmplitudeOscillator(float frequency, float low, float high);
	virtual ~AmplitudeOscillator();

	virtual float GetSample(float absoluteTime) = 0;

	float GetFrequency() const;
	float GetPeriod() const;
	float GetLow() const;
	float GetHigh() const;

private:

	float _frequency;
	float _period;
	float _low;
	float _high;
};

#endif