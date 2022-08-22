#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(BYTE* data, int bufferSize)
{
	_buffer.resize(bufferSize);
	memcpy(&_buffer[0], data, bufferSize);
}

SendBuffer::~SendBuffer()
{
	
}
