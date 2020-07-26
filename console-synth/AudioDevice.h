#ifndef AUDIO_DEVICE_H
#define AUDIO_DEFICE_H

#include <functional>
#include "RtAudio.h"
#include "Synth.h"
#include "Constant.h"

class AudioDevice
{
public:
    
    AudioDevice(Synth* synth);

    ~AudioDevice();

    static int AudioCallback(void* outputBuffer, void* inputBuffer,
                             unsigned int nFrames,
                             double streamTime,
                             RtAudioStreamStatus status,
                             void* userData);

    static Synth* GetSynth(AudioDevice* device);

    static float GetFrameTimeElapsed(AudioDevice* device);

private:

    // Primary RtAudio component
    RtAudio* _rtAudio;

    // "Readonly" Synth
    Synth* _synth;

    // Incremented during callback
    float _frameTimeElapsed;

};

#endif