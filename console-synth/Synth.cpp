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

Synth::Synth(const SynthConfiguration& configuration, unsigned int numberOfChannels, unsigned int samplingRate)
{
	_numberOfChannels = numberOfChannels;
	_samplingRate = samplingRate;

	this->Initialize(configuration);
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

void Synth::Initialize(const SynthConfiguration& configuration)
{
	// DESTRUCTOR!
	if (_configuration != nullptr)
		this->~Synth();

	_configuration = new SynthConfiguration(configuration);
	_pianoNotes = new std::map<int, SynthNote*>();

	_mixer = new Mixer();
	_delay = new Filter(configuration.GetDelaySeconds(), _numberOfChannels, _samplingRate);
	//_reverb = new Reverb(configuration.GetReverbDelaySeconds(), configuration.GetReverbGain(), samplingRate);
	_reverb = new AirwindowsEffect(configuration.GetReverbDelaySeconds(), configuration.GetReverbGain(), _numberOfChannels, _samplingRate);

	// MEMORY!!! These are being deleted in the Filter::~Filter destructor! Please watch memory carefully!!!
	for (int index = 0; index < _numberOfChannels; index++)
	{
		_delay->AddChannel((FilterChannelBase*)(new CombFilterChannel(configuration.GetDelaySeconds(), configuration.GetDelayGain(), configuration.GetDelayFeedback(), _samplingRate)));
	}
}

Synth::~Synth()
{
	delete _configuration;

	for (auto iter = _pianoNotes->begin(); iter != _pianoNotes->end(); ++iter)
	{
		delete iter->second;
	}

	delete _pianoNotes;
	delete _mixer;
	delete _delay;
	delete _reverb;
}

void Synth::SetConfiguration(const SynthConfiguration& configuration)
{
	this->Initialize(configuration);
}

void Synth::Set(int midiNumber, bool pressed, double absoluteTime)
{
	SynthNote* note = nullptr;

	// New Note
	if (!_pianoNotes->contains(midiNumber))
	{
		note = new SynthNote(midiNumber, *_configuration, _samplingRate);

		_pianoNotes->insert(std::make_pair(midiNumber, note));
	}

	else
	{
		note = _pianoNotes->at(midiNumber);
	}
	

	if (!pressed)
		note->DisEngage(absoluteTime);

	else if (!note->IsEngaged())
		note->Engage(absoluteTime);
}

void Synth::Clear(double absoluteTime)
{
	// Careful with iterator: cpp reference is not a bad place for "documentation"
	//
	for (auto iter = _pianoNotes->begin(); iter != _pianoNotes->end();)
	{
		if (!iter->second->HasOutput(absoluteTime))
		{
			// Deallocate synth note
			delete iter->second;

			// Remove from collection
			iter = _pianoNotes->erase(iter);
		}
		else
			++iter;
	}
}

bool Synth::IsSet(int midiNumber)
{
	return _pianoNotes->at(midiNumber)->IsEngaged();
}

bool Synth::HasNote(int midiNumber)
{
	return _pianoNotes->contains(midiNumber);
}

void Synth::GetSample(PlaybackFrame* frame, double absoluteTime)
{
	float output = 0;

	// BASE OSCILLATORS
	for (auto iter = _pianoNotes->begin(); iter != _pianoNotes->end(); ++iter)
	{
		SynthNote* note = iter->second;

		// Primary notes
		if (note->HasOutput(absoluteTime))
		{
			note->AddSample(frame, absoluteTime);
		}
	}

	// Dry Output -> Mixer -> Wet + Dry (Output)
	float dryOutput = output;
	float wetOutput = dryOutput;

	// OUTPUT EFFECTS	
	if (_configuration->GetHasReverb() && _reverb->HasOutput(absoluteTime))
		_reverb->GetSample(frame, absoluteTime);

	if (_configuration->GetHasDelay() && _delay->HasOutput(absoluteTime))
		_delay->GetSample(frame, absoluteTime);
}