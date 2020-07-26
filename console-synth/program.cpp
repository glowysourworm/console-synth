#include <iostream>
#include <string>

#include <Windows.h>

#include "Constant.h"
#include "Synth.h"
#include "SynthNote.h"
#include "RtAudio.h"


// Static variables for audio playback (callback is written as static)
Synth* _synth;
int _frameIndex;



void print(const char* str, bool newLine = true)
{
    if (!newLine)
        std::cout << str;
    else
        std::cout << str << std::endl;
}

void printHeader()
{
    print("Console Synthesizer");
    print("-------------------");
    print(" ");
    print("Waiting on input (Escape to exit)...");
}

Synth* configure(int* &usedCodes, int& usedCodesLength)
{
    SynthNote** notes = new SynthNote*[14];

    print("Initializing Synth Virtual Device...");

    notes[0] = new SynthNote("C3", "A", 48, WindowsKeyCodes::A);
    notes[1] = new SynthNote("C#3 / Db3", "W", 49, WindowsKeyCodes::W);
    notes[2] = new SynthNote("D3", "S", 50, WindowsKeyCodes::S);
    notes[3] = new SynthNote("D#3 / Eb3", "E", 51, WindowsKeyCodes::E);
    notes[4] = new SynthNote("E3", "D", 52, WindowsKeyCodes::D);
    notes[5] = new SynthNote("F3", "F", 53, WindowsKeyCodes::F);
    notes[6] = new SynthNote("F#3 / Gb3", "T", 54, WindowsKeyCodes::T);
    notes[7] = new SynthNote("G3", "G", 55, WindowsKeyCodes::G);
    notes[8] = new SynthNote("G#3 / Ab3", "Y", 56, WindowsKeyCodes::Y);
    notes[9] = new SynthNote("A4", "H", 57, WindowsKeyCodes::H);
    notes[10] = new SynthNote("A#4 / Bb4", "U", 58, WindowsKeyCodes::U);
    notes[11] = new SynthNote("B4", "J", 59, WindowsKeyCodes::J);
    notes[12] = new SynthNote("C4", "K", 60, WindowsKeyCodes::K);
    notes[13] = new SynthNote("D4", "L", 62, WindowsKeyCodes::L);

    // Setup key code array for return
    usedCodes = new int[14];
    usedCodesLength = 14;

    usedCodes[0] = WindowsKeyCodes::A;
    usedCodes[1] = WindowsKeyCodes::W;
    usedCodes[2] = WindowsKeyCodes::S;
    usedCodes[3] = WindowsKeyCodes::E;
    usedCodes[4] = WindowsKeyCodes::D;
    usedCodes[5] = WindowsKeyCodes::F;
    usedCodes[6] = WindowsKeyCodes::T;
    usedCodes[7] = WindowsKeyCodes::G;
    usedCodes[8] = WindowsKeyCodes::Y;
    usedCodes[9] = WindowsKeyCodes::H;
    usedCodes[10] = WindowsKeyCodes::U;
    usedCodes[11] = WindowsKeyCodes::J;
    usedCodes[12] = WindowsKeyCodes::K;
    usedCodes[13] = WindowsKeyCodes::L;

    return new Synth(notes, 14);
}

int callback(void* outputBuffer, void* inputBuffer,
             unsigned int nFrames,
             double streamTime,
             RtAudioStreamStatus status,
             void* userData)
{
    // Output frames should be interleved
    float* buffer = (float*)outputBuffer;
    float sampleSize = (1.0 / (float)SAMPLING_RATE);

    // Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
    for (unsigned int i = 0; i < nFrames; i++)
    {
        float absoluteTime = (++_frameIndex) * sampleSize;
        float sample = _synth->GetSample(absoluteTime);

        // Interleved frames
        for (unsigned int j = 0; j < NUMBER_CHANNELS; j++)
        {
            // Initialize sample to zero
            buffer[(2 * i) + j] = sample;
        }
    }

    if (_frameIndex >= MAXUINT32)
    {
        int foo = 2;
    }

    return 0;
}

RtAudio* initialize()
{
    print("Initializing audio playback device...");

    RtAudio* rtAudio = new RtAudio(RtAudio::Api::WINDOWS_WASAPI);

    RtAudio::StreamParameters parameters;
    unsigned int bufferFrames = AUDIO_BUFFER_SIZE;

    RtAudio::StreamOptions options;
    options.flags |= RTAUDIO_SCHEDULE_REALTIME;
    options.flags |= RTAUDIO_HOG_DEVICE;
    // options.flags |= RTAUDIO_NONINTERLEAVED;

    // STEREO / DEFAULT AUDIO DEVICE
    parameters.deviceId = rtAudio->getDefaultOutputDevice();
    parameters.firstChannel = 0;
    parameters.nChannels = NUMBER_CHANNELS;

    rtAudio->openStream(&parameters, NULL, RTAUDIO_FLOAT32, SAMPLING_RATE, &bufferFrames, &callback, NULL, &options);
    rtAudio->startStream();

    return rtAudio;
}

int main()
{
    bool exit = false;
    int* keyCodes;
    int keyCodesLength;

    // Create the predefined piano virtual device
    //    
    _synth = configure(keyCodes, keyCodesLength);

    // Initialize audio device
    RtAudio* device = initialize();

    // Print application header
    printHeader();

    // Listen for keyboard inputs / exit key
    while (!exit)
    {
        // Exit
        if (GetAsyncKeyState(VK_ESCAPE))
            exit = true;

        // Process primary synth input
        else
        {
            // Check the configured piano notes to process the input
            for (int i = 0; i < keyCodesLength; i++)
            {
                int code = keyCodes[i];

                // Update piano virtual device
                _synth->Set(code, (bool)GetAsyncKeyState(code), _frameIndex / (float)SAMPLING_RATE);
            }
        }
        
        Sleep(LOOP_INCREMENT);
    }

    // Delete memory for primary components
    //
    delete device;
    delete _synth;

    return 0;
}