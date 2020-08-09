#include "Envelope.h"

Envelope::Envelope(float attack, float decay, float sustain, float release, float attackPeak, float sustainPeak)
{
	_attack = attack;
	_decay = decay;
	_sustain = sustain;
	_release = release;

	_attackPeak = attackPeak;
	_sustainPeak = sustainPeak;

	_engaged = false;
	_hasEngaged = false;

	_disEngagedLevel = 0;
	_engagedTime = 0;
	_disEngagedTime = 0;
}
Envelope::~Envelope()
{

}

void Envelope::Engage(float absoluteTime)
{
	float currentLevel = GetEnvelopeLevel(absoluteTime);

	_engaged = true;
	_hasEngaged = true;

	// Set the engage time with an offset to smoothly transition envelopes
	if (currentLevel > 0)
	{
		// Subtract offset of the attack (ADVANCES ENVELOPE)
		_engagedTime = absoluteTime - ((currentLevel / _attackPeak) * _attack);
	}
	else
	{
		_engagedTime = absoluteTime;
	}
}

void Envelope::DisEngage(float absoluteTime)
{
	// Store envelope level to calculate release value
	//
	// CALL BEFORE DIS-ENGAGING
	//
	_disEngagedLevel = GetEnvelopeLevel(absoluteTime);

	_engaged = false;
	_disEngagedTime = absoluteTime;
}

bool Envelope::HasOutput(float absoluteTime)
{
	if (!_hasEngaged)
		return false;

	// Note is engaged
	if (_engaged)
		return true;

	// Check to see whether envelope is completed
	else
	{
		// Calculate time along the release
		float envelopeTime = absoluteTime - _disEngagedTime;

		return envelopeTime < _release;
	}
}

bool Envelope::IsEngaged()
{
	return _engaged;
}

float Envelope::GetEngageTime()
{
	return _engagedTime;
}

float Envelope::GetDisEngageTime()
{
	return _disEngagedTime;
}

float Envelope::GetEnvelopeLevel(float absoluteTime)
{
	if (!_hasEngaged)
		return 0;

	// Check piece-wise function to get envelope level
	//

	if (_engaged)
	{
		// Calculate time along the envelope
		float envelopeTime = absoluteTime - _engagedTime;

		// Attack
		if (envelopeTime < _attack)
		{
			return (_attackPeak / _attack) * envelopeTime;
		}
		// Decay
		else if (envelopeTime < _attack + _decay)
		{
			// Shift axis to put attack peak at zero
			return (((_sustainPeak - _attackPeak) / _decay) * (envelopeTime - _attack)) + _attackPeak;
		}
		// Sustain
		else
		{
			return _sustainPeak;
		}
	}
	else
	{
		// Calculate time along the RELEASE
		float envelopeTime = absoluteTime - _disEngagedTime;

		// Release
		if (envelopeTime < _release)
		{
			// Shift axis to put sustain peak at zero
			return (((-1 * _disEngagedLevel) / _release) * envelopeTime) + _disEngagedLevel;
		}
		else
			return 0;
	}
}