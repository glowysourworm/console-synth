#include "Envelope.h"

Envelope::Envelope()
{
	_attack = 0.1;
	_decay = 0.2;
	_sustain = 0.2;
	_release = 0.3;

	_attackPeak = 0.8;
	_sustainPeak = 0.5;

	_engaged = false;
	_hasEngaged = false;

	_disEngagedLevel = 0;
	_engagedTime = 0;
	_disEngagedTime = 0;
}

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
Envelope::Envelope(const Envelope& copy)
{
	_attack = copy.GetAttack();
	_decay = copy.GetDecay();
	_sustain = copy.GetSustain();
	_release = copy.GetRelease();

	_attackPeak = copy.GetAttackPeak();
	_sustainPeak = copy.GetSustainPeak();

	// Defaults
	_engaged = false;
	_hasEngaged = false;

	_disEngagedLevel = 0;
	_engagedTime = 0;
	_disEngagedTime = 0;
}
Envelope::~Envelope()
{

}

void Envelope::Set(const Envelope& envelope)
{
	_attack = envelope.GetAttack();
	_decay = envelope.GetDecay();
	_sustain = envelope.GetSustain();
	_release = envelope.GetRelease();

	_attackPeak = envelope.GetAttackPeak();
	_sustainPeak = envelope.GetSustainPeak();
}

void Envelope::Set(float attack, float decay, float sustain, float release, float attackPeak, float sustainPeak)
{
	_attack = attack;
	_decay = decay;
	_sustain = sustain;
	_release = release;

	_attackPeak = attackPeak;
	_sustainPeak = sustainPeak;
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
	if (!_engaged)
		return;

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

		// Check release time
		bool hasOutput = envelopeTime < _release;

		// Reset
		if (!hasOutput)
		{
			_hasEngaged = false;
			_engaged = false;
			_disEngagedLevel = 0;
			_engagedTime = 0;
			_disEngagedTime = 0;
		}

		return hasOutput;
	}
}

bool Envelope::IsEngaged()
{
	return _engaged;
}

float Envelope::GetAttack() const
{
	return _attack;
}
float Envelope::GetDecay() const
{
	return _decay;
}
float Envelope::GetSustain() const
{
	return _sustain;
}
float Envelope::GetRelease() const
{
	return _release;
}
float Envelope::GetAttackPeak() const
{
	return _attackPeak;
}
float Envelope::GetSustainPeak() const
{
	return _sustainPeak;
}

bool Envelope::operator!=(const Envelope& envelope)
{
	return !Compare(envelope);
}

bool Envelope::operator==(const Envelope& envelope)
{
	return Compare(envelope);
}

bool Envelope::Compare(const Envelope& envelope)
{
	return _attack == envelope.GetAttack() &&
		_sustain == envelope.GetSustain() &&
		_decay == envelope.GetDecay() &&
		_release == envelope.GetRelease() &&
		_attackPeak == envelope.GetAttackPeak() &&
		_sustainPeak == envelope.GetSustainPeak();
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