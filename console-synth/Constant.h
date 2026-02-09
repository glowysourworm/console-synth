#ifndef SYNTH_CONSTANT_H
#define SYNTH_CONSTANT_H

enum class AmplitudeOscillatorType : int {
	Sine = 0,
	Square,
	Triangle,
	Sawtooth,
	Random
};

enum class EnvelopeFilterType : int {
	Constant = 0,
	Oscillator,
	EnvelopeSweep
};

const int REVERB_COMB_SIZE = 4;
const int REVERB_ALLPASS_SIZE = 4;
const int MIDI_PIANO_LOW_NUMBER = 21;
const int MIDI_PIANO_HIGH_NUMBER = 108;
const int MIDI_PIANO_SIZE = MIDI_PIANO_HIGH_NUMBER - MIDI_PIANO_LOW_NUMBER + 1;
const float SIGNAL_LOW = -1;
const float SIGNAL_HIGH = 1;

#endif