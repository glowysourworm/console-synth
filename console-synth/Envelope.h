#ifndef ENVELOPE_H
#define ENVELOPE_H

class Envelope
{
public:
	Envelope(float attack, float decay, float sustain, float release, float attackPeak, float sustainPeak);
	~Envelope();

	void Engage(float absoluteTime);
	void DisEngage(float absoluteTime);
	bool HasOutput(float absoluteTime);
	bool IsEngaged();
	float GetEnvelopeLevel(float absoluteTime);
	float GetEngageTime();
	float GetDisEngageTime();

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

#endif ENVELOPE_H

