#include "CompressorChannel.h"
#include "Constant.h"
#include "Envelope.h"
#include "EnvelopeFilterChannel.h"
#include "PlaybackFrame.h"
#include "RandomOscillator.h"
#include "SawtoothOscillator.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "SynthConfiguration.h"
#include "SynthNote.h"
#include "TriangleOscillator.h"
#include <cmath>
#include <exception>

SynthNote::SynthNote(int midiNumber, const SynthConfiguration& configuration, unsigned int samplingRate)
{
	_midiNumber = midiNumber;
	_envelope = new Envelope(configuration.GetNoteEnvelope());

	_envelopeFilter = new EnvelopeFilterChannel(1.0, 		
		configuration.GetEnvelopeFilterCutoff(),
		configuration.GetEnvelopeFilterResonance(),
		configuration.GetEnvelopeFilterType(),
		configuration.GetEnvelopeFilterOscillatorType(),
		configuration.GetEnvelopeFilterOscillatorFrequency(),
		configuration.GetEnvelopeFilter(), samplingRate);

	_compressor = new CompressorChannel(configuration.GetCompressorGain(), 
		samplingRate,
		configuration.GetCompressorThreshold(), 
		configuration.GetCompressionRatio(), 
		configuration.GetCompressorRelaxationPeriod(), 
		configuration.GetCompressorAttack(), 
		configuration.GetCompressorRelease());

	_envelopeFilterEnabled = configuration.GetHasEnvelopeFilter();
	_compressorEnabled = configuration.GetHasCompressor();

	// Initialize Oscillator
	switch (configuration.GetOscillatorType())
	{
	case OscillatorType::Sine:
		_oscillator = new SineOscillator(this->GetFrequency());
		break;
	case OscillatorType::Random:
		_oscillator = new RandomOscillator(this->GetFrequency(), 4);
		break;
	case OscillatorType::Square:
		_oscillator = new SquareOscillator(this->GetFrequency());
		break;
	case OscillatorType::Triangle:
		_oscillator = new TriangleOscillator(this->GetFrequency());
		break;
	case OscillatorType::Sawtooth:
		_oscillator = new SawtoothOscillator(this->GetFrequency());
		break;
	default:
		throw std::exception("Unhandled Oscillator Type:  SynthNote.cpp");
	}
}

SynthNote::~SynthNote()
{
	delete _envelope;
}

int SynthNote::GetMidiNumber() const
{
	return _midiNumber;
}

void SynthNote::ApplyImpl(PlaybackFrame* frame, float absoluteTime, bool overwriteOrAdd) const
{
	// Generate Oscillator
	if (overwriteOrAdd)
		_oscillator->GetSample(frame, absoluteTime);
	else
		_oscillator->MixSample(frame, absoluteTime);

	for (int index = 0; index < frame->GetChannelCount(); index++)
	{
		// Envelope
		float output = _envelope->GetEnvelopeLevel(absoluteTime) * frame->GetSample(index);

		// FILTER SWEEP: Check envelope filter for output
		if (_envelopeFilterEnabled && _envelopeFilter->HasOutput(absoluteTime))
		{
			output = _envelopeFilter->Apply(output, absoluteTime);
		}

		// COMPRESSOR: Apply Compression
		if (_compressorEnabled && _compressor->HasOutput(absoluteTime))
		{
			output = _compressor->Apply(output, absoluteTime);
		}

		frame->SetSample(index, output);
	}
}

void SynthNote::GetSample(PlaybackFrame* frame, float absoluteTime) const
{
	this->ApplyImpl(frame, absoluteTime, true);
}

void SynthNote::AddSample(PlaybackFrame* frame, float absoluteTime) const
{
	this->ApplyImpl(frame, absoluteTime, false);
}

float SynthNote::GetFrequency() const
{
	return 440 * powf(2, ((_midiNumber - 69.0) / 12.0));
}

bool SynthNote::IsEngaged()
{
	return _envelope->IsEngaged();
}

bool SynthNote::HasOutput(float absoluteTime)
{
	return _envelope->HasOutput(absoluteTime);
}

void SynthNote::Engage(float absoluteTime)
{
	_envelope->Engage(absoluteTime);
	_envelopeFilter->Engage(absoluteTime);
}

void SynthNote::DisEngage(float absoluteTime)
{
	_envelope->DisEngage(absoluteTime);
	_envelopeFilter->DisEngage(absoluteTime);
}
