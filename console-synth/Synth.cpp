#include "Synth.h"

Synth::Synth(SynthNote** pianoNotes, int pianoNotesLength)
{
	_pianoNotes = new std::list<SynthNote*>();

	SynthNote** iter = pianoNotes;

	for (int i=0;i<pianoNotesLength;i++)
		_pianoNotes->push_back(pianoNotes[i]);

	_filter = new ButterworthFilter(SAMPLING_RATE, 1.0);
	_filterEnvelope = new Envelope(1.5, 0.5, 0.5, 3.0, 1, 0.8);
	_reverb = new Reverb(0.01, 0.5, SAMPLING_RATE);
}

Synth::~Synth()
{
	delete _pianoNotes;
	delete _filterEnvelope;
	delete _reverb;
}

// Getters
const SynthNote* Synth::Get(int keyCode)
{
	std::list<SynthNote*>::iterator iter = _pianoNotes->begin();

	for (iter = _pianoNotes->begin(); iter != _pianoNotes->end(); iter++)
	{
		if ((*iter)->GetKeyCode() == keyCode)
			return *iter;
	}

	// TODO: C++ Exceptions...
	throw;
}

float Synth::GetSample(float absoluteTime)
{
	//if (_noteEnvelope->HasOutput(absoluteTime))
	//	return _noteEnvelope->GetEnvelopeLevel(absoluteTime) * this->GenerateTriangle(absoluteTime);

	//else
	//	return 0;

	std::list<SynthNote*>::iterator iter = _pianoNotes->begin();	

	float output = 0;

	// BASE OSCILLATORS
	for (iter = _pianoNotes->begin(); iter != _pianoNotes->end(); iter++)
	{
		// Engaged notes will have a non-zero output
		if ((*iter)->GetEnvelopeLevel(absoluteTime) > 0)
		{
			output += (*iter)->GetEnvelopeLevel(absoluteTime) * GenerateTriangle(absoluteTime, (*iter)->GetFrequency());
		}
	}

	// FILTER SWEEP
	//if (_filterEnvelope->HasOutput(absoluteTime))
	//{
	//	_filter->Set((float)MAX_FREQUENCY * _filterEnvelope->GetEnvelopeLevel(absoluteTime), 0.5);

	//	output = _filter->Apply(output);
	//}

	// OUTPUT EFFECTS
	output = _reverb->Apply(output);

	return output;
}

// Setters
void Synth::Set(int keyCode, bool pressed, float absoluteTime)
{
	std::list<SynthNote*>::iterator iter = _pianoNotes->begin();

	bool anyPressed = false;

	for (iter = _pianoNotes->begin(); iter != _pianoNotes->end(); iter++)
	{
		if ((*iter)->GetKeyCode() == keyCode &&
			(*iter)->GetPressed() != pressed)
			(*iter)->SetPressed(pressed, absoluteTime);

		anyPressed |= (*iter)->GetPressed();
	}

	// Set filter envelope
	if (anyPressed && !_filterEnvelope->IsEngaged())
		_filterEnvelope->Engage(absoluteTime);
	
	else if (!anyPressed && _filterEnvelope->IsEngaged())
		_filterEnvelope->DisEngage(absoluteTime);
}

// BASE OSCILLATORS
float Synth::GenerateSine(float absoluteTime, float frequency)
{
	return sinf(2.0 * PI * frequency * absoluteTime);
}

float Synth::GenerateTriangle(float absoluteTime, float frequency)
{
	float period = 1.0 / frequency;
	float periodQuarter = 0.25 * period;

	// Using modulo arithmetic to get the relative period time
	float periodTime = fmodf(absoluteTime, period);

	// First Quadrant
	if (periodTime < periodQuarter)
	{
		return (4.0 / period) * periodTime;
	}

	// Second Quadrant
	else if (periodTime < 2.0 * periodQuarter)
	{
		return ((-4.0 / period) * (periodTime - periodQuarter)) + 1;
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