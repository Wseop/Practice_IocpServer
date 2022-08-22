#include "pch.h"
#include "RecvBuffer.h"

using namespace std;

RecvBuffer::RecvBuffer(int bufferSize) : _readPos(0), _writePos(0), _bufferSize(bufferSize), _capacity(bufferSize* BUFFER_COUNT)
{
	// bufferSize보다 여유있게 할당 해둠
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

bool RecvBuffer::OnRead(int numOfBytes)
{
	// readPos 이동
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int numOfBytes)
{
	// writePos 이동
	if (numOfBytes > FreeSize())
		return false;

	_writePos += numOfBytes;
	return true;
}

void RecvBuffer::Clean()
{
	// read, write pos 초기화
	int dataSize = DataSize();
	if (dataSize == 0)
	{
		_readPos = 0;
		_writePos = 0;
	}
	else if (FreeSize() < _bufferSize)
	{
		// 읽을 데이터가 남아있고, 여유공간이 얼마 남지 않았을 경우 데이터 복사와 함께 pos 초기화
		memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
		_readPos = 0;
		_writePos = dataSize;
	}
}
