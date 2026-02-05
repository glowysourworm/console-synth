#include "CombFilter.h"
#include "Compressor.h"
#include "Constant.h"
#include "Mixer.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"
#include <map>
#include <utility>

Synth::Synth(const SynthConfiguration& configuration)
{
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
	_delay = new CombFilter(configuration.GetDelaySeconds(), 0.8, SAMPLING_RATE, configuration.GetDelayFeedback());
	_compressor = new Compressor(10, SAMPLING_RATE, configuration.GetCompressorThreshold(), configuration.GetCompressionRatio(), configuration.GetCompressorRelaxationPeriod(), configuration.GetCompressorAttack(), configuration.GetCompressorRelease());
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
	delete _compressor;
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
		note = new SynthNote(midiNumber, *_configuration);

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

			// Remove from the mixer
			_mixer->ClearChannel(iter->first);

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
	if (_pianoNotes->size() == 0)
		return 0;

	// BASE OSCILLATORS
	for (auto iter = _pianoNotes->begin(); iter != _pianoNotes->end(); ++iter)
	{
		SynthNote* note = iter->second;
		float output = 0;

		// Primary notes
		if (note->HasOutput(absoluteTime))
		{
			output += note->GetSample(absoluteTime);
		}

		// Send to the mixer
		_mixer->SetChannel(note->GetMidiNumber(), output);
	}

	// Mixer -> AMPLITUDE OSCILLATOR (LFO)
	float dryOutput = _mixer->Get();
	float wetOutput = dryOutput;

	// OUTPUT EFFECTS	
	if (_configuration->GetHasDelay())
		wetOutput = _delay->Apply(wetOutput, absoluteTime);

	if (_configuration->GetHasCompressor())
		wetOutput = _compressor->Apply(wetOutput, absoluteTime);

	return wetOutput;
}