#pragma once
class FilterBase
{
public:
	FilterBase(float gain, int samplingRate);
	virtual ~FilterBase();

	virtual float Apply(float sample, float absoluteTime) = 0;

public:

	float GetSamplingRate() const;
	float GetGain() const;

private:

	float _samplingRate;
	float _gain;
};

