#include "SynthNoteMap.h"
#include "WindowsKeyCodes.h"
#include <exception>
#include <map>
#include <utility>

SynthNoteMap::SynthNoteMap()
{
	_keyCodeMap = new std::map<WindowsKeyCodes, int>();
	_keyCodeReverseMap = new std::map<int, WindowsKeyCodes>();
}
SynthNoteMap::SynthNoteMap(const SynthNoteMap& copy)
{
	_keyCodeMap = new std::map<WindowsKeyCodes, int>();
	_keyCodeReverseMap = new std::map<int, WindowsKeyCodes>();

	SynthNoteMap* that = this;

	copy.Iterate([&that](WindowsKeyCodes keyCode, int midiNote) {
		that->Add(keyCode, midiNote);
	});
}
SynthNoteMap::~SynthNoteMap()
{
	delete _keyCodeMap;
	delete _keyCodeReverseMap;
}

void SynthNoteMap::Add(WindowsKeyCodes keyCode, int midiNote)
{
	if (_keyCodeMap->contains(keyCode))
		throw new std::exception("Key code already contained within the note map");

	_keyCodeMap->insert(std::make_pair(keyCode, midiNote));
	_keyCodeReverseMap->insert(std::make_pair(midiNote, keyCode));
}

WindowsKeyCodes SynthNoteMap::GetKeyCode(int midiNote) const
{
	if (!_keyCodeReverseMap->contains(midiNote))
		throw new std::exception("Key code not contained within the note map");

	return _keyCodeReverseMap->at(midiNote);
}
int SynthNoteMap::GetMidiNote(WindowsKeyCodes keyCode) const
{
	if (!_keyCodeMap->contains(keyCode))
		throw new std::exception("Key code not contained within the note map");

	return _keyCodeMap->at(keyCode);
}

bool SynthNoteMap::HasMidiNote(WindowsKeyCodes keyCode) const
{
	return _keyCodeMap->contains(keyCode);
}

void SynthNoteMap::Iterate(KeymapIterationCallback callback) const
{
	for (auto iter = _keyCodeMap->begin(); iter != _keyCodeMap->end(); ++iter)
	{
		callback(iter->first, iter->second);
	}
}
