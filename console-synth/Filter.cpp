#include "Filter.h"
#include "FilterBase.h"
#include "FilterChannelBase.h"
#include "PlaybackFrame.h"
#include <exception>
#include <vector>

Filter::Filter(float gain, unsigned int numberOfChannels, unsigned int samplingRate) : FilterBase(gain, numberOfChannels, samplingRate)
{
	_channels = new std::vector<FilterChannelBase*>();
}

Filter::~Filter()
{
	// MEMORY!! These are being new'd in another class! Please check references to Filter::AddChannel
	for (int index = 0; index < _channels->size(); index++)
	{
		delete _channels->at(index);
	}

	delete _channels;
}

void Filter::AddChannel(FilterChannelBase* filter)
{
	_channels->push_back(filter);
}

void Filter::GetSample(PlaybackFrame* frame, float absoluteTime)
{
	if (frame->GetChannelCount() != _channels->size())
		throw new std::exception("Filter channel count does not match the playback channel count!");

	for (int index = 0; index < this->GetNumberOfChannels(); index++)
	{
		// Apply Channel
		float sample = _channels->at(index)->Apply(frame->GetSample(index), absoluteTime);

		// Set Frame Channel
		frame->SetSample(index, sample);
	}
}

bool Filter::HasOutput(float absoluteTime) const
{
	if (this->GetNumberOfChannels() != _channels->size())
		throw new std::exception("Filter channel count does not match the playback channel count!");

	bool hasOutput = false;

	for (int index = 0; index < this->GetNumberOfChannels() && !hasOutput; index++)
	{
		hasOutput = _channels->at(index)->HasOutput(absoluteTime);
	}

	return hasOutput;
}
