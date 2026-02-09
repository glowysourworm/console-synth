#include "ButterworthFilter.h"
#include "ButterworthFilterChannel.h"
#include "FilterBase.h"
#include "PlaybackFrame.h"
#include <cmath>

ButterworthFilter::ButterworthFilter(float gain, unsigned int numberOfChannels, unsigned int samplingRate) : FilterBase(gain, numberOfChannels, samplingRate)
{
	_filterChannels = new ButterworthFilterChannel*[numberOfChannels];

	for (int index = 0; index < numberOfChannels; index++)
	{
		_filterChannels[index] = new ButterworthFilterChannel(samplingRate, gain);
	}

	this->Set(samplingRate / 2.0f, 0.0);
}

ButterworthFilter::~ButterworthFilter()
{
	for (int index = 0; index < this->GetNumberOfChannels(); index++)
	{
		delete _filterChannels[index];
	}

	delete[] _filterChannels;
}

void ButterworthFilter::Set(float cutoffFrequency, float resonance)
{
	for (int index = 0; index < this->GetNumberOfChannels(); index++)
	{
		_filterChannels[index]->Set(cutoffFrequency, resonance);
	}
}

void ButterworthFilter::GetSample(PlaybackFrame* frame, float absoluteTime)
{
	for (int index = 0; index < this->GetNumberOfChannels(); index++)
	{
		// Apply Channel Filter
		float sample = _filterChannels[index]->Apply(frame->GetSample(index), absoluteTime);

		// Set Frame Channel
		frame->SetSample(index, sample);
	}
}

bool ButterworthFilter::HasOutput(float absoluteTime) const
{
	return true;
}
