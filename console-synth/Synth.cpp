#include "CombFilter.h"
#include "Compressor.h"
#include "Constant.h"
#include "Mixer.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"
#include <vector>

Synth::Synth(int midiLow, int midiHigh, const SynthConfiguration& configuration)
{
	_configuration = new SynthConfiguration(configuration);
	_pianoNotes = new std::vector<SynthNote*>();

	_midiHigh = midiHigh;
	_midiLow = midiLow;

	_mixer = new Mixer(midiHigh - midiLow + 1);
	_delay = new CombFilter(configuration.GetDelaySeconds(), 0.8, SAMPLING_RATE, configuration.GetDelayFeedback());
	_compressor = new Compressor(10, SAMPLING_RATE, configuration.GetCompressorThreshold(), configuration.GetCompressionRatio(), configuration.GetCompressorRelaxationPeriod(), configuration.GetCompressorAttack(), configuration.GetCompressorRelease());

	// Initialize Piano
	for (int midiNote = midiLow; midiNote <= midiHigh; midiNote++)
	{
		_pianoNotes->push_back(new SynthNote(midiNote, configuration));
	}
}

Synth::~Synth()
{
	delete _configuration;

	for (int i = 0; i < _pianoNotes->size(); i++)
	{
		delete _pianoNotes->at(i);
	}

	delete[] _pianoNotes;
	delete _mixer;
	delete _envelopeFilter;
	delete _delay;
	delete _compressor;
}

void Synth::Set(int midiNumber, bool pressed, double absoluteTime)
{
	SynthNote* note = _pianoNotes->at(midiNumber - _midiLow);

	if (!pressed)
		note->DisEngage(absoluteTime);

	else if (!note->IsEngaged())
		note->Engage(absoluteTime);
}

bool Synth::IsSet(int midiNumber)
{
	return _pianoNotes->at(midiNumber - _midiLow)->IsEngaged();
}

float Synth::GetSample(double absoluteTime)
{
	if (_pianoNotes->size() == 0)
		return 0;

	// BASE OSCILLATORS
	for (int i = _pianoNotes->size() - 1; i >= 0; i--)
	{
		SynthNote* note = _pianoNotes->at(i);
		float output = 0;

		// Primary notes
		if (note->HasOutput(absoluteTime))
		{
			output += note->GetSample(absoluteTime);
		}

		// Send to the mixer
		_mixer->SetChannel(i, output);
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