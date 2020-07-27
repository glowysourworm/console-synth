#include "Synth.h"

Synth::Synth(SynthNote** pianoNotes, int pianoNotesLength)
{
	_pianoNotes = pianoNotes;
	_pianoNotesLength = pianoNotesLength;

	_frequencyShift = 0.5;
	_frequencyShiftGain = 0.4;

	_filter = new ButterworthFilter(SAMPLING_RATE, 1.0);
	_filterEnvelope = new Envelope(1.5, 0.5, 0.5, 3.0, 1, 0.8);
	_reverb = new Reverb(0.1, 0.5, SAMPLING_RATE);
	_delay = new CombFilter(0.65, 0.25, SAMPLING_RATE);
}

Synth::~Synth()
{
	delete[] _pianoNotes;
	delete _filterEnvelope;
	delete _reverb;
}

// Getters
SynthNote* Synth::Get(int keyCode)
{
	for (int i = 0; i < _pianoNotesLength; i++)
		if (_pianoNotes[i]->GetKeyCode() == keyCode)
			return _pianoNotes[i];

	throw;
}

float Synth::GetSample(float absoluteTime)
{
	float output = 0;

	// BASE OSCILLATORS
	for (int i=0;i<_pianoNotesLength;i++)
	{
		SynthNote* note = _pianoNotes[i];

		// Primary notes
		output += (1 - _frequencyShiftGain) *
				  note->GetEnvelopeLevel(absoluteTime) * 
				  GenerateTriangle(absoluteTime, note->GetFrequency());

		// Frequency Shift Effect
		output += _frequencyShiftGain * 
				  note->GetEnvelopeLevel(absoluteTime) * 
				  GenerateTriangle(absoluteTime, note->GetFrequency() * _frequencyShift);
	}

	// FILTER SWEEP
	if (_filterEnvelope->HasOutput(absoluteTime))
	{
		_filter->Set((float)MAX_FREQUENCY * _filterEnvelope->GetEnvelopeLevel(absoluteTime), 0.3);

		output = _filter->Apply(output);
	}

	// OUTPUT EFFECTS	
	// float reverbOutput = _reverb->Apply(output);
	// float delayOutput = _delay->Apply(output);

	// return _delay->Apply(output);

	// return _reverb->Apply(output);

	// return 0.5 * (reverbOutput + delayOutput);

	// return reverbOutput;

	// return delayOutput;

	return output;
}

// Setters
void Synth::Set(int keyCode, bool pressed, float absoluteTime)
{
	// NOTE*** MUST BE SYNCHRONIZED COLLECTION ITERATION
	SynthNote* note = this->Get(keyCode);

	if (note->GetPressed() != pressed)
	{
		note->SetPressed(pressed, absoluteTime);

		// Check for new key press
		if (pressed)
			_filterEnvelope->Engage(absoluteTime);
	}
}
void Synth::SetDisEngaged(float absoluteTime)
{
	_filterEnvelope->DisEngage(absoluteTime);
}

// BASE OSCILLATORS
float Synth::GenerateSine(float absoluteTime, float frequency)
{
	return sinf(2.0 * PI * frequency * absoluteTime);
}

float Synth::GenerateTriangle(float absoluteTime, float frequency)
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
float Synth::GenerateSawtooth(float absoluteTime, float frequency)
{
	float period = 1.0 / frequency;

	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, period);

	return ((2.0 / period) * periodTime) - 1;
}
float Synth::GenerateSquare(float absoluteTime, float frequency)
{
	float period = 1.0 / frequency;

	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, period);

	if (periodTime < period / 2.0)
		return 1;

	else
		return -1;
}