#pragma once

#include "WindowsKeyCodes.h"
#include <functional>
#include <map>

class SynthNoteMap
{
public:

	using KeymapIterationCallback = std::function<void(WindowsKeyCodes keyCode, int midiNote)>;

	SynthNoteMap();
	SynthNoteMap(const SynthNoteMap& copy);
	~SynthNoteMap();

	void Add(WindowsKeyCodes keyCode, int midiNote);

	WindowsKeyCodes GetKeyCode(int midiNote) const;
	int GetMidiNote(WindowsKeyCodes keyCode) const;
	bool HasMidiNote(WindowsKeyCodes keyCode) const;

	/// <summary>
	/// Iterates the keymap with the provided callback
	/// </summary>
	void Iterate(KeymapIterationCallback callback) const;

private:

	// These are maps from the key code -> midi number (and reverse)
	std::map<WindowsKeyCodes, int>* _keyCodeMap;
	std::map<int, WindowsKeyCodes>* _keyCodeReverseMap;
};

