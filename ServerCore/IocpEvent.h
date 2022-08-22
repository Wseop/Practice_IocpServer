#pragma once

#include "SendBuffer.h"

class IocpObject;
class Session;

enum class EventType
{
	Connect,
	Disconnect,
	Accept,
	Send,
	Recv,
};

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);
	~IocpEvent();

	void Init();
	EventType GetType() { return _type; }
	void SetOwner(std::shared_ptr<IocpObject> owner) { _owner = owner; }
	std::shared_ptr<IocpObject> GetOwner() { return _owner; }

private:
	EventType _type;
	std::shared_ptr<IocpObject> _owner;
};

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) {}
};

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) {}
};

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

	void SetSession(std::shared_ptr<Session> session) { _session = session; }
	std::shared_ptr<Session> GetSession() { return _session; }

private:
	std::shared_ptr<Session> _session;
};

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send), _sendBuffer(nullptr) {}
	~SendEvent();

	void SetSendBuffer(class SendBuffer* sendBuffer) { _sendBuffer = sendBuffer; }
	SendBuffer* GetSendBuffer() { return _sendBuffer; }

private:
	class SendBuffer* _sendBuffer;
};

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};