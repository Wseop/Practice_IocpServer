#pragma once

#include <vector>

class SendBuffer
{
public:
	SendBuffer(BYTE* data, int bufferSize);
	~SendBuffer();

	size_t WriteSize() { return _buffer.size(); }
	BYTE* GetBuffer() { return _buffer.data(); }

private:
	std::vector<BYTE> _buffer;
};

