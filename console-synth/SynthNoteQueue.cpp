#include "PlaybackFrame.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"
#include "SynthNoteQueue.h"
#include <map>
#include <utility>
#include <vector>

SynthNoteQueue::SynthNoteQueue(int capacity)
{
	_capacity = capacity;

	_engagedNotes = new std::map<int, SynthNote*>();
	_disengagedNotes = new std::vector<SynthNote*>();
}

SynthNoteQueue::~SynthNoteQueue()
{
	for (auto iter = _engagedNotes->begin(); iter != _engagedNotes->end(); ++iter)
	{
		delete iter->second;
	}
	for (int index = 0; index < _disengagedNotes->size(); index++)
	{
		delete _disengagedNotes->at(index);
	}
	delete _engagedNotes;
	delete _disengagedNotes;
}

bool SynthNoteQueue::SetNote(int midiNumber, bool pressed, double absoluteTime, const SynthConfiguration* configuration, unsigned int samplingRate) const
{
	if (pressed)
	{
		// Already Engaged
		if (_engagedNotes->contains(midiNumber))
			return true;

		// Engage
		else if (_engagedNotes->size() < _capacity)
		{
			// (MEMORY!) Delete notes when they're removed from the dis-engaged list
			SynthNote* note = new SynthNote(configuration, midiNumber, samplingRate);

			note->Engage(absoluteTime);

			_engagedNotes->insert(std::make_pair(midiNumber, note));

			return true;
		}

		// Queue is full
		else
			return false;
	}
	else
	{
		// Not Engaged
		if (!_engagedNotes->contains(midiNumber))
			return true;

		else
		{
			SynthNote* note = _engagedNotes->at(midiNumber);

			note->DisEngage(absoluteTime);

			// Remove Engaged Note
			_engagedNotes->erase(midiNumber);

			// Disengaged
			_disengagedNotes->push_back(note);

			return true;
		}
	}
}

bool SynthNoteQueue::SetSample(PlaybackFrame* frame, double absoluteTime, double gain, double leftRight)
{
	// Use local frame to mix the note output
	PlaybackFrame noteFrame(frame->GetChannelCount());

	// Engaged
	for (auto iter = _engagedNotes->begin(); iter != _engagedNotes->end(); ++iter)
	{
		iter->second->AddSample(&noteFrame, absoluteTime);
	}

	// Disengaged (also prune collection)
	for (int index = _disengagedNotes->size() - 1; index >= 0; index--)
	{
		// HasOutput (ringing out of the note)
		if (_disengagedNotes->at(index)->HasOutput(absoluteTime))
			_disengagedNotes->at(index)->AddSample(&noteFrame, absoluteTime);

		// ...Empty
		else
		{
			// (MEMORY!) These are completed notes
			delete _disengagedNotes->at(index);

			_disengagedNotes->erase(_disengagedNotes->begin() + index);
		}
			
	}

	// MIXING THIS RIGHT AWAY FOR NOW
	for (int index = 0; index < noteFrame.GetChannelCount(); index++)
	{			
		// Left
		if (index < noteFrame.GetChannelCount() / 2)
			noteFrame.SetSample(index, (noteFrame.GetSample(index) + noteFrame.GetSample(index)) * gain * (1 - leftRight));

		// Right
		else
			noteFrame.SetSample(index, (noteFrame.GetSample(index) + noteFrame.GetSample(index)) * gain * leftRight);
	}

	frame->SetFrame(&noteFrame);

	return _engagedNotes->size() > 0 || _disengagedNotes->size() > 0;
}
