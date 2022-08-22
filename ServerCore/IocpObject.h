#pragma once

#include <vector>
#include "RecvBuffer.h"
#include "IocpEvent.h"

class IocpObject : public std::enable_shared_from_this<IocpObject>
{
public:
	IocpObject();
	virtual ~IocpObject();

	virtual void Dispatch(IocpEvent* iocpEvent, DWORD bytesTransferred) abstract;
};

// AcceptEvent 담당
class Listener : public IocpObject
{
	const int ACCEPT_COUNT = 5;

public:
	Listener();
	~Listener();

	void Dispatch(IocpEvent* iocpEvent, DWORD bytesTransferred) override;

	void StartAccept();
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET _listenSocket;
	SOCKADDR_IN _serverAddr;
	std::vector<AcceptEvent*> _acceptEvents;
};

class Session : public IocpObject
{
	const int BUFFER_SIZE = 0x10000;    // 64KB

public:
	Session();
	~Session();

	void Dispatch(IocpEvent* iocpEvent, DWORD bytesTransferred) override;

	SOCKET GetSocket() { return _clientSocket; }
	void SetAddr(SOCKADDR_IN sockAddr) { _sockAddr = sockAddr; }
	SOCKADDR_IN GetAddr() { return _sockAddr; }
	RecvBuffer& GetRecvBuffer() { return _recvBuffer; }

	void StartConnect();
	void RegisterConnect();
	void ProcessConnect();

	void StartSend(class SendBuffer* sendBuffer);
	void RegisterSend(SendEvent* sendEvent);
	void ProcessSend(SendEvent* sendEvent, DWORD bytesTransferred);

	void RegisterRecv();
	void ProcessRecv(DWORD bytesTransferred);
	void OnRecv(DWORD bytesTransferred);

private:
	SOCKET _clientSocket;
	SOCKADDR_IN _sockAddr;
	ConnectEvent* _connectEvent;
	RecvEvent _recvEvent;
	RecvBuffer _recvBuffer;
};