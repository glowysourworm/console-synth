#include "AudioPlayer.h"
#include "MidiFilePlayer.h"
#include "RtAudio.h"

MidiFilePlayer::MidiFilePlayer()
{
	_device = NULL;
}

MidiFilePlayer::~MidiFilePlayer()
{
	AudioPlayer::~AudioPlayer();
}

bool MidiFilePlayer::Initialize(RtAudioCallback audioCallback, RtAudioErrorCallback errorCallback)
{
	return AudioPlayer::Initialize(audioCallback, errorCallback) && _device->Initialize();
}

bool MidiFilePlayer::Tick()
{
	//MidiFile midi;

	//midi.read(std::string(fileName));

	//// Set "absolute ticks mode"
	//midi.absoluteTicks();

	//// Finally, try merging tracks to create a single-track song
	//// midiFile.joinTracks();

	//float secondsPerTick = midi.getFileDurationInSeconds() / midi.getFileDurationInTicks();
	//float timeCursor = 0;

	//// Calculate number of midi ticks to progress during this callback
	//// double midiTicksPerFrame = (((1 / (double)NUMBER_CHANNELS) / (double)SAMPLING_RATE) * ticksPerSecond);

	//_waveBufferLength = midi.getFileDurationInSeconds() * SAMPLING_RATE;
	//int waveIndex = 0;
	//int samplesPerTick = (int)(secondsPerTick * SAMPLING_RATE);

	//_waveBuffer = new float[_waveBufferLength];

	//for (int k = 0; k < midi.getFileDurationInTicks(); k++)
	//{
	//	for (int i = 0; i < midi.getTrackCount(); i++)
	//	{
	//		if (k >= midi[i].size())
	//			continue;

	//		MidiEvent midiEvent = midi[i][k];

	//		// Note On
	//		if (midiEvent.isNoteOn())
	//		{
	//			_synth->Set(midiEvent.getKeyNumber(), true, timeCursor);
	//		}

	//		// Note Off
	//		else if (midiEvent.isNoteOff())
	//		{
	//			_synth->Set(midiEvent.getKeyNumber(), false, timeCursor);
	//		}
	//	}

	//	// Advance midi time clock
	//	timeCursor += secondsPerTick;

	//	// Synthesize samples during that time period
	//	for (int i = waveIndex; (i < (waveIndex + samplesPerTick)) && (i < _waveBufferLength); i++)
	//	{
	//		_waveBuffer[i] = _synth->GetSample(i / (float)SAMPLING_RATE);
	//	}

	//	// Increment wave index
	//	waveIndex += samplesPerTick;
	//}

	return true;
}

bool MidiFilePlayer::Close()
{
	return AudioPlayer::Close();
}
