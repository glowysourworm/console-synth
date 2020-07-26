#ifndef SYNTHNOTE_H
#define SYNTHNOTE_H

#include "WindowsKeyCodes.h"
#include "Envelope.h"
#include <math.h>
#include <atomic>

class SynthNote
{
public:

	SynthNote() {};

	SynthNote(const char* name,
		const char* keyMap,
		int midiNumber,
		int keyCode);

	~SynthNote();

	const char* GetName();
	const char* GetKeyMap();
	const int GetKeyCode();
	const bool GetPressed();
	const float GetFrequency();
	float GetEnvelopeLevel(float absoluteTime);

	void SetPressed(bool pressed, float absoluteTime);

private:

	const char* _name;
	const char* _keyMap;
	bool  _pressed;
	float _pressedTime;
	int _keyCode;
	int  _midiNumber;

	Envelope* _envelope;
};

#endif