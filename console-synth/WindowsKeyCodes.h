#ifndef WINDOWS_KEY_CODES_H
#define WINDOWS_KEY_CODES_H


// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
//
// KEY CODES THAT AREN'T DEFINED IN WINUSER.H (??)
//
enum class WindowsKeyCodes : int
{
	NUMBER_0 = 0x30,
	NUMBER_1 = 0x31,
	NUMBER_2 = 0x32,
	NUMBER_3 = 0x33,
	NUMBER_4 = 0x34,
	NUMBER_5 = 0x35,
	NUMBER_6 = 0x36,
	NUMBER_7 = 0x37,
	NUMBER_8 = 0x38,
	NUMBER_9 = 0x39,

	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5A,
	

	SEMICOLON = 0x80,			// VK_OEM_1
	PLUS = 0xBB,				// VK_OEM_PLUS
	QUESTION_MARK = 0xBF,		// VK_OEM_2
	LEFT_BRACKET = 0xDB,		// VK_OEM_4 (left bracket)
	BACKSLASH = 0xDC,			// VK_OEM_5 (backslash / pipe)
	RIGHT_BRACKET = 0xDD,		// VK_OEM_6 (right bracket)	
	APOSTROPHE = 0xDE,			// VK_OEM_7
	COMMA = 0xBC,				// VK_OEM_COMMA
	MINUS = 0xBD,				// VK_OEM_MINUS
	PERIOD = 0xBE				// VK_OEM_PERIOD
};

#endif