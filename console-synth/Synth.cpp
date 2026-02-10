#include "AirwindowsEffect.h"
#include "CombFilterChannel.h"
#include "Constant.h"
#include "Filter.h"
#include "FilterChannelBase.h"
#include "Mixer.h"
#include "PlaybackFrame.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"
#include <map>
#include <utility>

Synth::Synth(const SynthConfiguration* configuration, unsigned int numberOfChannels, unsigned int samplingRate)
{
	_configuration = configuration;
	_numberOfChannels = numberOfChannels;
	_samplingRate = samplingRate;

	_pianoNotes = new std::map<int, SynthNote*>();

	_mixer = new Mixer(configuration->GetOutputGain(), configuration->GetOutputLeftRight(), numberOfChannels);
	_delay = new Filter(configuration->GetDelaySeconds(), _numberOfChannels, _samplingRate);
	//_reverb = new Reverb(configuration.GetReverbDelaySeconds(), configuration.GetReverbGain(), samplingRate);
	_reverb = new AirwindowsEffect(configuration->GetReverbDelaySeconds(), configuration->GetReverbGain(), _numberOfChannels, _samplingRate);

	// MEMORY!!! These are being deleted in the Filter::~Filter destructor! Please watch memory carefully!!!
	for (int index = 0; index < _numberOfChannels; index++)
	{
		_delay->AddChannel((FilterChannelBase*)(new CombFilterChannel(configuration->GetDelaySeconds(), configuration->GetDelayGain(), configuration->GetDelayFeedback(), _samplingRate)));
	}

	for (int midiNumber = configuration->GetMidiLow(); midiNumber <= configuration->GetMidiHigh(); midiNumber++)
	{
		// Synth Notes
		_pianoNotes->insert(std::make_pair(midiNumber, new SynthNote(configuration, midiNumber, _samplingRate)));

		// Mixer Channels (output parameters are not for individual channels)
		_mixer->SetChannel(midiNumber, 1.0f, 0.5f);
	}
}

void Synth::GetNotes(int array[MIDI_PIANO_SIZE], int& arrayLength) const
{
	int index = 0;

	// Get Number of Active Notes
	arrayLength = _pianoNotes->size();

	for (auto iter = _pianoNotes->begin(); iter != _pianoNotes->end(); ++iter)
	{
		array[index++] = iter->first;
	}
}

Synth::~Synth()
{
	for (auto iter = _pianoNotes->begin(); iter != _pianoNotes->end(); ++iter)
	{
		delete iter->second;
	}

	delete _pianoNotes;
	delete _mixer;
	delete _delay;
	delete _reverb;
}

void Synth::SetConfiguration(const SynthConfiguration* configuration)
{
	_configuration = configuration;

	_delay->SetConfiguration(configuration);
	_reverb->SetConfiguration(configuration);
	_mixer->Set(configuration->GetOutputGain(), configuration->GetOutputLeftRight());
	
	// Synth Notes
	for (int midiNumber = configuration->GetMidiLow(); midiNumber <= configuration->GetMidiHigh(); midiNumber++)
	{
		_pianoNotes->at(midiNumber)->SetConfiguration(configuration);
	}
}

void Synth::Set(int midiNumber, bool pressed, double absoluteTime)
{
	if (!pressed)
		_pianoNotes->at(midiNumber)->DisEngage(absoluteTime);

	else if (!_pianoNotes->at(midiNumber)->IsEngaged())
		_pianoNotes->at(midiNumber)->Engage(absoluteTime);
}

bool Synth::IsSet(int midiNumber)
{
	return _pianoNotes->at(midiNumber)->IsEngaged();
}

bool Synth::HasNote(int midiNumber)
{
	return _pianoNotes->contains(midiNumber);
}

bool Synth::GetSample(PlaybackFrame* frame, double absoluteTime)
{
	bool hasOutput = false;
	float gain = _configuration->GetOutputGain();
	float leftRight = _configuration->GetOutputLeftRight();

	// BASE OSCILLATORS
	for (auto iter = _pianoNotes->begin(); iter != _pianoNotes->end(); ++iter)
	{
		int midiNumber = iter->second->GetMidiNumber();

		// Primary notes (apply to the mixer)
		if (iter->second->HasOutput(absoluteTime))
		{
			hasOutput = true;

			// Mixer Channel
			PlaybackFrame* mixerFrame = _mixer->GetFrame(midiNumber);

			// Create Sound Sample (for channel)
			iter->second->GetSample(mixerFrame, absoluteTime);

			// MIXING THIS RIGHT AWAY FOR NOW
			for (int index = 0; index < frame->GetChannelCount(); index++)
			{			
				// Left
				if (index < frame->GetChannelCount() / 2)
					frame->SetSample(index, (mixerFrame->GetSample(index) + frame->GetSample(index)) * gain * (1 - leftRight));

				// Right
				else
					frame->SetSample(index, (mixerFrame->GetSample(index) + frame->GetSample(index)) * gain * leftRight);
			}
		}
	}

	// Dry Output -> Mixer -> Wet + Dry (Output)
	//_mixer->MixOutput(frame);

	// OUTPUT EFFECTS	
	if (_configuration->GetHasReverb() && _reverb->HasOutput(absoluteTime))
		_reverb->GetSample(frame, absoluteTime);

	if (_configuration->GetHasDelay() && _delay->HasOutput(absoluteTime))
		_delay->GetSample(frame, absoluteTime);

	return hasOutput;
}