#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

template <typename T>
class CircularQueue
{
public:
	CircularQueue<T>()
	{
		_array = new T[0];
		_size = 0;
		_front = 0;
		_back = 0;
	}

	CircularQueue<T>(int capacity)
	{
		_array = new T[capacity];
		_capacity = capacity;
		_size = 0;
		_front = 0;
		_back = 0;
	}

	~CircularQueue<T>()
	{
		delete[] _array;
	}

	void Enqueue(T item)
	{
		if (_size < _capacity)
		{

		}
		else
			throw;
	}

	T Dequeue()
	{

	}

	int RemainingCapacity()
	{
		return _capacity - _size;
	}

private:
	T* _array;
	int _capacity;
	int _size;

	int _front;
	int _back;
};


#endif

