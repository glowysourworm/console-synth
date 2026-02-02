#include "AmplitudeOscillator.h"
#include "ButterworthFilter.h"
#include "CombFilter.h"
#include "Compressor.h"
#include "Constant.h"
#include "Envelope.h"
#include "Mixer.h"
#include "Reverb.h"
#include "Synth.h"
#include "SynthNote.h"
#include <cmath>
#include <vector>

Synth::Synth(int midiLow, int midiHigh, const Envelope& noteEnvelope)
{
	_pianoNotes = new std::vector<SynthNote*>();

	_frequencyShift = 0.5;
	_frequencyShiftGain = 0.5;

	_midiHigh = midiHigh;
	_midiLow = midiLow;

	_mixer = new Mixer(midiHigh - midiLow + 1);
	_oscillator = new AmplitudeOscillator(2.5, AmplitudeOscillatorType::Sine);
	_filter = new ButterworthFilter(SAMPLING_RATE, 1.0);
	_filterEnvelope = new Envelope(noteEnvelope);
	_reverb = new Reverb(0.1, 0.9, SAMPLING_RATE);
	_delay = new CombFilter(1, 0.8, SAMPLING_RATE);
	_limiter = new Compressor(10, 0.6, 4, 0.2, 0.2, 0.2, true);

	// Initialize Piano
	for (int midiNote = midiLow; midiNote <= midiHigh; midiNote++)
	{
		_pianoNotes->push_back(new SynthNote(midiNote, noteEnvelope));
	}
}

Synth::~Synth()
{
	for (int i = 0; i < _pianoNotes->size(); i++)
	{
		delete _pianoNotes->at(i);
	}

	delete[] _pianoNotes;
	delete _filterEnvelope;
	delete _reverb;
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
			output += note->GetEnvelopeLevel(absoluteTime) * GenerateSine(absoluteTime, note->GetFrequency());
		}


		// Send to the mixer
		_mixer->SetChannel(i, output);
	}

	// Mixer -> AMPLITUDE OSCILLATOR (LFO)
	float dryOutput = _mixer->Get();
	float wetOutput = dryOutput;

	// FILTER SWEEP
	//if (_filterEnvelope->HasOutput(absoluteTime))
	//{
	//_filter->Set(fabs((float)(MAX_FREQUENCY * GenerateTriangle(absoluteTime, 3))), 0.15);
	//_filter->Set((float)MAX_FREQUENCY * _filterEnvelope->GetEnvelopeLevel(absoluteTime), 0.9);

	//wetOutput = _filter->Apply(wetOutput);
	//}

	// OUTPUT EFFECTS	
	wetOutput = _delay->Apply(wetOutput);
	//wetOutput = _reverb->Apply(wetOutput);

	//return _limiter->Apply(absoluteTime, wetOutput);
	//return _mixer->Get();

	return wetOutput;
}

// BASE OSCILLATORS
float Synth::GenerateSine(double absoluteTime, float frequency)
{
	return sinf(2.0 * PI * frequency * absoluteTime);
}
float Synth::GenerateTriangle(double absoluteTime, float frequency)
{
	float period = 1.0 / (float)frequency;
	float periodQuarter = 0.25f * period;

	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, period);

	// First Quadrant
	if (periodTime < periodQuarter)
	{
		return (4.0 / period) * periodTime;
	}

	// Second Quadrant
	else if (periodTime < (2.0 * periodQuarter))
	{
		return ((-4.0 / period) * (periodTime - (float)periodQuarter)) + 1;
	}

	// Third Quadrant
	else if (periodTime < 3.0 * periodQuarter)
	{
		return (-4.0 / period) * (periodTime - (2.0 * periodQuarter));
	}

	// Fourth Quadrant
	else
	{
		return ((4.0 / period) * (periodTime - (3.0 * periodQuarter))) - 1;
	}
}
float Synth::GenerateSawtooth(double absoluteTime, float frequency)
{
	float period = 1.0 / frequency;

	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, period);

	return ((2.0 / period) * periodTime) - 1;
}
float Synth::GenerateSquare(double absoluteTime, float frequency)
{
	float period = 1.0 / frequency;

	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, period);

	if (periodTime < period / 2.0)
		return 1;

	else
		return -1;
}