#pragma once
#include "Constant.h"
#include "OscillatorBase.h"
#include "PlaybackFrame.h"

class TriangleOscillator : public OscillatorBase
{
public:

	TriangleOscillator(float frequency);
	~TriangleOscillator() override;

	float GetMonoSample(float absoluteTime) override;

	OscillatorType GetType() const override;
};

