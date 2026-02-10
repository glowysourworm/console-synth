#pragma once
#include "Constant.h"
#include "OscillatorBase.h"
#include "PlaybackFrame.h"

class SawtoothOscillator : public OscillatorBase
{
public:

	SawtoothOscillator(float frequency);
	~SawtoothOscillator() override;

	float GetMonoSample(float absoluteTime) override;

	OscillatorType GetType() const override;
};

