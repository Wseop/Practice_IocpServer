#pragma once

#include <vector>

class RecvBuffer
{
	const int BUFFER_COUNT = 10;

public:
	RecvBuffer(int bufferSize);
	~RecvBuffer();

	int FreeSize() { return _capacity - _writePos; }
	int DataSize() { return _writePos - _readPos; }

	bool OnRead(int numOfBytes);
	bool OnWrite(int numOfBytes);
	void Clean();

	BYTE* WritePos() { return &_buffer[_writePos]; }
	BYTE* ReadPos() { return &_buffer[_readPos]; }

private:
	std::vector<BYTE> _buffer;
	int _readPos;
	int _writePos;
	int _bufferSize;
	int _capacity;
};

