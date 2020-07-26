#include "SynthNote.h"

SynthNote::SynthNote(const char* name,
				     const char* keyMap,
 					 int midiNumber,
  					 int keyCode)
{
	_name = name;
	_keyMap = keyMap;
	_midiNumber = midiNumber;
	_pressed = false;
	_pressedTime = 0;
	_keyCode = keyCode;
	_envelope = new Envelope(0.1, 0.2, 0.5, 3, 1, 0.8);
}

SynthNote::~SynthNote()
{
	delete _envelope;
}

const char* SynthNote::GetName()
{
	return _name;
}

const char* SynthNote::GetKeyMap()
{
	return _keyMap;
}

const int SynthNote::GetKeyCode()
{
	return _keyCode;
}

const bool SynthNote::GetPressed()
{
	return _pressed;
}

float SynthNote::GetEnvelopeLevel(float absoluteTime)
{
	return _envelope->GetEnvelopeLevel(absoluteTime);
}

const float SynthNote::GetFrequency()
{
	return 440 * powf(2, ((_midiNumber - 69.0) / 12.0));
}

void SynthNote::SetPressed(bool pressed, float absoluteTime)
{
	_pressed = pressed;
	_pressedTime = absoluteTime;
	
	if (pressed)
		_envelope->Engage(absoluteTime);
	else
		_envelope->DisEngage(absoluteTime);
}