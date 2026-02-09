#include "AirwindowsEffect.h"
#include "FilterBase.h"
#include "PlaybackFrame.h"
#include <kCathedral.h>

AirwindowsEffect::AirwindowsEffect(float delaySeconds, float gain, unsigned int numberOfChannels, unsigned int samplingRate) : FilterBase(gain, numberOfChannels, samplingRate)
{
	_effect = new kCathedral(0);
	_input = new float* [2];
	_output = new float* [2];

	_delay = delaySeconds;

	// One sample per channel
	_input[0] = new float[1];			// Left
	_input[1] = new float[1];			// Right

	_output[0] = new float[1];			// Left (output)
	_output[1] = new float[1];			// Right (output)

	_effect->setSampleRate(samplingRate);
	_effect->setParameter(0, delaySeconds);
}

AirwindowsEffect::~AirwindowsEffect()
{
	delete _effect;
}

void AirwindowsEffect::GetSample(PlaybackFrame* frame, float absoluteTime)
{
	// airwindows format:  The effect treats the inputs as dry input, and the outputs as the result. I think 
	//					   they may have been confused about other audio APIs, which have input as the microphone.
	//
	//					   He's also left the effects "window" as essentially however many frames you want to
	//					   process at once, but then assumed that there's only one frame! So, I'm unsure what 
	//					   he wanted.. But, I hope I've applied the effect properly!
	// 
	//					   Finally, the audio is non-interleved. So, you'll have to know to parse your signal 
	//					   before calling his plugin. We're going to spool off samples one-by-one, with a pre-allocated
	//					   single-sample buffer.
	// 
	//					   Let's see how it sounds!
	//

	_input[0][0] = frame->GetSample(0);
	_input[1][0] = frame->GetSample(1);

	_effect->processReplacing(_input, _output, 1);

	frame->SetSample(0, _output[0][0]);
	frame->SetSample(0, _output[1][0]);
}

bool AirwindowsEffect::HasOutput(float absoluteTime) const
{
	return true;
}
