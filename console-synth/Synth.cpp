#include "Synth.h"

Synth::Synth()
{
	_pianoNotes = new std::vector<SynthNote*>();

	_frequencyShift = 0.5;
	_frequencyShiftGain = 0.5;

	_mixer = new Mixer(1);
	_oscillator = new AmplitudeOscillator(2.5, AmplitudeOscillatorType::Sine);
	_filter = new ButterworthFilter(SAMPLING_RATE, 1.0);
	_filterEnvelope = new Envelope(1.5, 3, 0.5, 1, 0.8, 0.1);
	_reverb = new Reverb(0.1, 0.9, SAMPLING_RATE);
	_delay = new CombFilter(0.1, 0.35, SAMPLING_RATE);
	_limiter = new Compressor(10, 0.6, 4, 0.2, 0.2, 0.2);	
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

void Synth::Set(int midiNumber, bool pressed, float absoluteTime)
{
	for (int i = 0; i < _pianoNotes->size(); i++)
	{
		SynthNote* note = _pianoNotes->at(i);

		if (note->GetMidiNumber() == midiNumber)
		{
			if (!pressed)
				note->DisEngage(absoluteTime);

			else
				note->Engage(absoluteTime);

			// Don't assign new note
			return;
		}
	}

	// Assign new note
	SynthNote* note = new SynthNote(midiNumber);

	_pianoNotes->push_back(note);
}

float Synth::GetSample(float absoluteTime)
{
	if (_pianoNotes->size() == 0)
		return 0;

	float output = 0;

	// BASE OSCILLATORS
	for (int i=_pianoNotes->size() - 1;i >= 0; i--)
	{
		SynthNote* note = _pianoNotes->at(i);

		if (note->IsEngaged())
			output += GenerateTriangle(absoluteTime, note->GetFrequency());

		// GROOM COLLECTION
		else
		{
			// delete note;
			// _pianoNotes->pop_back();
		}

			// Primary notes
			//output += (1 - _frequencyShiftGain) *
			//		  note->GetEnvelopeLevel(absoluteTime) * 
			//		  GenerateTriangle(absoluteTime, note->GetFrequency());

			//// Frequency Shift Effect
			//output += _frequencyShiftGain * 
			//		  note->GetEnvelopeLevel(absoluteTime) * 
			//		  GenerateSine(absoluteTime, note->GetFrequency() * _frequencyShift);

			//// Average the two signals
			//output *= 0.5;

			// Send to the mixer
			// _mixer->SetChannel(i, output);
	}

	// Mixer -> AMPLITUDE OSCILLATOR (LFO)
	 // float dryOutput = _mixer->Get();
	 // float wetOutput = dryOutput;

	// FILTER SWEEP
	//if (_filterEnvelope->HasOutput(absoluteTime))
	//{
	//	_filter->Set((float)MAX_FREQUENCY * _filterEnvelope->GetEnvelopeLevel(absoluteTime), 0.3);

	//	wetOutput = _filter->Apply(wetOutput);
	//}

	// OUTPUT EFFECTS	
	// wetOutput = _delay->Apply(wetOutput);
	// wetOutput = _reverb->Apply(wetOutput);

	// return _limiter->Apply(absoluteTime, wetOutput);
	 return output;
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