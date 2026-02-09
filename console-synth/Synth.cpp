#include "AirwindowsEffect.h"
#include "CombFilter.h"
#include "Constant.h"
#include "Mixer.h"
#include "Reverb.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"
#include <map>
#include <utility>

Synth::Synth(const SynthConfiguration& configuration, unsigned int samplingRate)
{
	this->Initialize(configuration, samplingRate);
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

void Synth::Initialize(const SynthConfiguration& configuration, unsigned int samplingRate)
{
	// DESTRUCTOR!
	if (_configuration != nullptr)
		this->~Synth();

	_configuration = new SynthConfiguration(configuration);
	_pianoNotes = new std::map<int, SynthNote*>();

	_mixer = new Mixer();
	_delay = new CombFilter(configuration.GetDelaySeconds(), 0.8, samplingRate, configuration.GetDelayFeedback());
	//_reverb = new Reverb(configuration.GetReverbDelaySeconds(), configuration.GetReverbGain(), samplingRate);
	_reverb = new AirwindowsEffect(configuration.GetReverbDelaySeconds(), configuration.GetReverbGain(), samplingRate);
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

void Synth::SetConfiguration(const SynthConfiguration& configuration, unsigned int samplingRate)
{
	this->Initialize(configuration, samplingRate);
}

void Synth::Set(int midiNumber, bool pressed, double absoluteTime, unsigned int samplingRate)
{
	SynthNote* note = nullptr;

	// New Note
	if (!_pianoNotes->contains(midiNumber))
	{
		note = new SynthNote(midiNumber, *_configuration, samplingRate);

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

float Synth::GetSample(double absoluteTime)
{
	float output = 0;

	// BASE OSCILLATORS
	for (auto iter = _pianoNotes->begin(); iter != _pianoNotes->end(); ++iter)
	{
		SynthNote* note = iter->second;

		// Primary notes
		if (note->HasOutput(absoluteTime))
		{
			output += note->GetSample(absoluteTime);
		}
	}

	// Dry Output -> Mixer -> Wet + Dry (Output)
	float dryOutput = output;
	float wetOutput = dryOutput;

	// OUTPUT EFFECTS	
	if (_configuration->GetHasReverb() && _reverb->HasOutput(absoluteTime))
		wetOutput = _reverb->Apply(wetOutput, absoluteTime);

	if (_configuration->GetHasDelay() && _delay->HasOutput(absoluteTime))
		wetOutput = _delay->Apply(wetOutput, absoluteTime);

	return wetOutput;
}