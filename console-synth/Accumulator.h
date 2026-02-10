#pragma once

#include "PlaybackBuffer.h"

template<SignalValue TSignal>
class Accumulator
{
public:

	Accumulator();
	Accumulator(int windowLength);
	~Accumulator();

	void Add(TSignal value);

	TSignal GetTotal() const;
	TSignal GetAvg() const;

	void Reset();

private:

	TSignal _last;
	TSignal _lastWindowed;
	TSignal _average;
	TSignal _total;
	int _counter;
	int _windowLength;

};

template<SignalValue TSignal>
Accumulator<TSignal>::Accumulator()
{
	_last = 0;
	_lastWindowed = 0;
	_average = 0;
	_total = 0;
	_counter = 0;
	_windowLength = -1;
}

template<SignalValue TSignal>
Accumulator<TSignal>::Accumulator(int windowLength)
{
	_last = 0;
	_lastWindowed = 0;
	_average = 0;
	_total = 0;
	_counter = 0;
	_windowLength = windowLength;
}

template<SignalValue TSignal>
Accumulator<TSignal>::~Accumulator()
{
}

template<SignalValue TSignal>
void Accumulator<TSignal>::Add(TSignal value)
{
	// Windowed
	if (_windowLength > 0 && _counter >= _windowLength)
	{
		_average += (1 / (double)(_counter)) * (value - _lastWindowed);
		_lastWindowed = value;
	}

	// Cummulative
	else
	{
		_average += (value - _average) / (_counter++ + 1);
	}


	_last = value;
	_total += value;
}

template<SignalValue TSignal>
TSignal Accumulator<TSignal>::GetTotal() const
{
	return _total;
}

template<SignalValue TSignal>
TSignal Accumulator<TSignal>::GetAvg() const
{
	return _average;
}

template<SignalValue TSignal>
void Accumulator<TSignal>::Reset()
{
	_last = 0;
	_average = 0;
	_total = 0;
}
