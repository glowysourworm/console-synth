#pragma once
#include "Constant.h"
#include "OscillatorBase.h"
#include "PlaybackFrame.h"

class SquareOscillator : public OscillatorBase
{
public:

	SquareOscillator(float frequency);
	~SquareOscillator() override;

	float GetMonoSample(float absoluteTime) override;

	OscillatorType GetType() const override;
};

