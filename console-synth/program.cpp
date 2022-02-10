#include <iostream>
#include <string>

#include <Windows.h>

#include "Constant.h"
#include "Synth.h"
#include "SynthNote.h"
#include "RtAudio.h"
#include "MidiFile.h"

using namespace smf;

// Static variables for audio playback (callback is written as static)
Synth* _synth;
int _frameIndex;
bool configuredForMidi;
bool _initialized;
float* _waveBuffer;
int _waveBufferLength;

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

int callback(void* outputBuffer, void* inputBuffer,
             unsigned int nFrames,
             double streamTime,
             RtAudioStreamStatus status,
             void* userData)
{
    if (!_initialized)
        return 0;

    // Output frames should be interleved
    float* buffer = (float*)outputBuffer;
    float sampleSize = (1.0 / (float)SAMPLING_RATE);
    float systemTime = (float)_frameIndex / (float)SAMPLING_RATE;

    // Calculate frame data (BUFFER SIZE = NUMBER OF CHANNELS x NUMBER OF FRAMES)
    for (unsigned int i = 0; (i < nFrames) && (_frameIndex < _waveBufferLength); i++)
    {
        float absoluteTime = (_frameIndex++) * sampleSize;
        // float sample = _synth->GetSample(absoluteTime);
        float sample = _waveBuffer[_frameIndex + i];

        // Interleved frames
        for (unsigned int j = 0; j < NUMBER_CHANNELS; j++)
        {
            // Initialize sample to zero
            buffer[(2 * i) + j] = sample;
        }
    }

    return 0;
}

void errorCallback(RtAudioError::Type type, const std::string& errorText)
{
    print(errorText.c_str(), true);
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
    options.flags |= RTAUDIO_MINIMIZE_LATENCY;
    // options.flags |= RTAUDIO_NONINTERLEAVED;

    // STEREO / DEFAULT AUDIO DEVICE
    parameters.deviceId = rtAudio->getDefaultOutputDevice();
    parameters.firstChannel = 0;
    parameters.nChannels = NUMBER_CHANNELS;

    rtAudio->openStream(&parameters, NULL, RTAUDIO_FLOAT32, SAMPLING_RATE, &bufferFrames, &callback, NULL, &options, errorCallback);
    rtAudio->startStream();

    return rtAudio;
}

void beginKeyboardInputLoop()
{

}

void beginMidiFilePlayback(char* fileName)
{
    MidiFile midi;

    midi.read(std::string(fileName));

    // Set "absolute ticks mode"
    midi.absoluteTicks();

    // Finally, try merging tracks to create a single-track song
    // midiFile.joinTracks();

    float secondsPerTick = midi.getFileDurationInSeconds() / midi.getFileDurationInTicks();
    float timeCursor = 0;

    // Calculate number of midi ticks to progress during this callback
    // double midiTicksPerFrame = (((1 / (double)NUMBER_CHANNELS) / (double)SAMPLING_RATE) * ticksPerSecond);

    _waveBufferLength = midi.getFileDurationInSeconds() * SAMPLING_RATE;
    int waveIndex = 0;
    int samplesPerTick = (int)(secondsPerTick * SAMPLING_RATE);

    _waveBuffer = new float[_waveBufferLength];

    for (int k = 0; k < midi.getFileDurationInTicks(); k++)
    {
        for (int i = 0; i < midi.getTrackCount(); i++)
        {
            if (k >= midi[i].size())
                continue;

            MidiEvent midiEvent = midi[i][k];

            // Note On
            if (midiEvent.isNoteOn())
            {
                _synth->Set(midiEvent.getKeyNumber(), true, timeCursor);
            }

            // Note Off
            else if (midiEvent.isNoteOff())
            {
                _synth->Set(midiEvent.getKeyNumber(), false, timeCursor);
            }
        }

        // Advance midi time clock
        timeCursor += secondsPerTick;

        // Synthesize samples during that time period
        for (int i = waveIndex; (i < (waveIndex + samplesPerTick)) && (i < _waveBufferLength); i++)
        {
            _waveBuffer[i] = _synth->GetSample(i / (float)SAMPLING_RATE);
        }

        // Increment wave index
        waveIndex += samplesPerTick;
     }
}

int main(int argc, char* argv[], char* envp[])
{
    // Create the predefined piano virtual device
    //    
    _synth = new Synth();

    // Initialize audio device
    RtAudio* device = initialize();

    // Print application header
    printHeader();

    // Read midi file
    if (argc > 1)
    {        
        configuredForMidi = true;
        beginMidiFilePlayback(argv[1]);
    }
    // Manual keyboard input
    else
    {        
        configuredForMidi = false;
        beginKeyboardInputLoop();        
    }

    // Unblock primary audio device callback
    _initialized = true;

    bool exit = false;

    // Listen for keyboard inputs / exit key
    while (!exit)
    {
        // Exit
        if (GetAsyncKeyState(VK_ESCAPE))
            exit = true;

        Sleep(LOOP_INCREMENT);
    }

    // Delete memory for primary components
    //
    delete device;
    delete _synth;

    return 0;
}