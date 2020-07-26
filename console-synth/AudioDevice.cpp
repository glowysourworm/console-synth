#include "AudioDevice.h"

AudioDevice::AudioDevice(Synth* synth)
{
    _rtAudio = new RtAudio(/*RtAudio::Api::WINDOWS_DS*/);
    _synth = synth;

    RtAudio::StreamParameters parameters;
    unsigned int bufferFrames = AUDIO_BUFFER_SIZE;

    RtAudio::StreamOptions options;
    options.flags |= RTAUDIO_SCHEDULE_REALTIME;
    options.flags |= RTAUDIO_HOG_DEVICE;
    // options.flags |= RTAUDIO_NONINTERLEAVED;

    // STEREO / DEFAULT AUDIO DEVICE
    parameters.deviceId = _rtAudio->getDefaultOutputDevice();
    parameters.firstChannel = 0;
    parameters.nChannels = NUMBER_CHANNELS;

    _rtAudio->openStream(&parameters, NULL, RTAUDIO_FLOAT32, SAMPLING_RATE, &bufferFrames, &AudioDevice::AudioCallback, NULL, &options);
    _rtAudio->startStream();
}

AudioDevice::~AudioDevice()
{
    _rtAudio->closeStream();

    delete _rtAudio;
}

// STATIC
Synth* AudioDevice::GetSynth(AudioDevice* device)
{
    return device->_synth;
}

float AudioDevice::GetFrameTimeElapsed(AudioDevice* device)
{
    return device->_frameTimeElapsed;
}

// PRIMARY RT AUDIO CALLBACK
int AudioDevice::AudioCallback(void* outputBuffer, void* inputBuffer,
    unsigned int nFrames,
    double streamTime,
    RtAudioStreamStatus status,
    void* userData)
{
    // Output frames should be interleved
    float* buffer = (float*)outputBuffer;
    float sampleSize = (1.0 / (float)SAMPLING_RATE);

    // Synth notes for processing
    SynthNote* notes = _synth->GetNotes();

    // Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
    for (unsigned int i = 0; i < nFrames; i++)
    {
        float frameTime = i * sampleSize;

        // Interleved frames
        for (unsigned int j = 0; j < NUMBER_CHANNELS; j++)
        {
            // Iterate synth notes to create output
            for (unsigned int k = 0; k < _synth->GetNotesLength(); k++)
            {
                // Note is engaged
                if (notes[k].GetPressed())
                {
                    // Set frame data (L / R)
                    buffer[(2 * i) + j] += notes[k].GetVoice()->GetSample(_frameTimeElapsed + frameTime);
                }
            }
        }
    }

    _frameTimeElapsed += sampleSize * nFrames;

    return 0;
}