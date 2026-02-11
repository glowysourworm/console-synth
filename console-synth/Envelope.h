#pragma once

class Envelope
{
public:
	Envelope();
	Envelope(float attack, float decay, float sustain, float release, float attackPeak, float sustainPeak);
	Envelope(const Envelope& copy);
	~Envelope();

	void Set(const Envelope& envelope);
	void Set(float attack, float decay, float sustain, float release, float attackPeak, float sustainPeak);

	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);
	bool HasOutput(float absoluteTime);
	bool IsEngaged();
	float GetEnvelopeLevel(float absoluteTime);
	float GetEngageTime();
	float GetDisEngageTime();

	float GetAttack() const;
	float GetDecay() const;
	float GetSustain() const;
	float GetRelease() const;
	float GetAttackPeak() const;
	float GetSustainPeak() const;

	bool operator!=(const Envelope& envelope);
	bool operator==(const Envelope& envelope);

private:

	bool Compare(const Envelope& envelope);

private:

	// Times in seconds
	float _attack;
	float _decay;
	float _sustain;
	float _release;

	// Amplitude fractions of the primary system AMPLITUDE  (0 < level <= 1)
	float _attackPeak;
	float _sustainPeak;

	// Values stored when SynthNote is pressed / released
	//

	bool _engaged;	  // Set to TRUE when synth note is pressed
	bool _hasEngaged; // Set to TRUE after first engagement

	// Absolute time values when voice is engaged / dis-engaged
	//
	float _engagedTime;
	float _disEngagedTime;
	float _disEngagedLevel;
};
