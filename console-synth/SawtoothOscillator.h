#pragma once
#include "OscillatorBase.h"
#include "PlaybackFrame.h"

class SawtoothOscillator : public OscillatorBase
{
public:

	SawtoothOscillator(float frequency);
	~SawtoothOscillator() override;

	float GetMonoSample(float absoluteTime) override;
};

