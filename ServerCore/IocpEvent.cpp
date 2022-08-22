#include "pch.h"
#include "IocpEvent.h"

IocpEvent::IocpEvent(EventType type) : _type(type)
{
	Init();
}

IocpEvent::~IocpEvent()
{
}

void IocpEvent::Init()
{
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
}

SendEvent::~SendEvent()
{
	if (_sendBuffer != nullptr)
	{
		delete _sendBuffer;
		_sendBuffer = nullptr;
	}
}
