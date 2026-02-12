#include "AirwindowsEffect.h"
#include "CombFilterChannel.h"
#include "Filter.h"
#include "FilterChannelBase.h"
#include "PlaybackFrame.h"
#include "Synth.h"
#include "SynthConfiguration.h"
#include "SynthNoteQueue.h"

Synth::Synth(const SynthConfiguration* configuration, unsigned int numberOfChannels, unsigned int samplingRate)
{
	_numberOfChannels = numberOfChannels;
	_samplingRate = samplingRate;

	_pianoNotes = new SynthNoteQueue(10);

	_delay = new Filter(configuration->GetDelaySeconds(), _numberOfChannels, _samplingRate);
	//_reverb = new Reverb(configuration.GetReverbDelaySeconds(), configuration.GetReverbGain(), samplingRate);
	_reverb = new AirwindowsEffect(configuration->GetReverbDelaySeconds(), configuration->GetReverbGain(), _numberOfChannels, _samplingRate);

	// MEMORY!!! These are being deleted in the Filter::~Filter destructor! Please watch memory carefully!!!
	for (int index = 0; index < _numberOfChannels; index++)
	{
		_delay->AddChannel((FilterChannelBase*)(new CombFilterChannel(configuration->GetDelaySeconds(), configuration->GetDelayGain(), configuration->GetDelayFeedback(), _samplingRate)));
	}
}

Synth::~Synth()
{
	delete _pianoNotes;
	delete _delay;
	delete _reverb;
}

void Synth::SetConfiguration(const SynthConfiguration* configuration)
{
	_delay->SetConfiguration(configuration);
	_reverb->SetConfiguration(configuration);
}

void Synth::Set(int midiNumber, bool pressed, double absoluteTime, const SynthConfiguration* configuration)
{
	_pianoNotes->SetNote(midiNumber, pressed, absoluteTime, configuration, _samplingRate);
}
bool Synth::GetSample(PlaybackFrame* frame, double absoluteTime, const SynthConfiguration* configuration)
{
	float gain = configuration->GetOutputGain();
	float leftRight = configuration->GetOutputLeftRight();

	// Primary Synth Voice(s)
	bool hasOutput = _pianoNotes->SetSample(frame, absoluteTime, gain, leftRight);

	// Effect Chain
	bool reverbHasOutput = _reverb->HasOutput(absoluteTime) && configuration->GetHasReverb();
	bool delayHasOutput = _delay->HasOutput(absoluteTime) && configuration->GetHasDelay();

	// OUTPUT EFFECTS	
	if (reverbHasOutput)
		_reverb->GetSample(frame, absoluteTime);

	if (delayHasOutput)
		_delay->GetSample(frame, absoluteTime);

	return hasOutput || reverbHasOutput || delayHasOutput;
}