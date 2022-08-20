#pragma once

#include <vector>
class IocpEvent;
class AcceptEvent;
class ConnectEvent;

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
public:
	Session();
	~Session();

	void Dispatch(IocpEvent* iocpEvent, DWORD bytesTransferred) override;

	SOCKET GetSocket() { return _clientSocket; }
	void SetAddr(SOCKADDR_IN sockAddr) { _sockAddr = sockAddr; }
	SOCKADDR_IN GetAddr() { return _sockAddr; }

	void StartConnect();
	void RegisterConnect();
	void ProcessConnect();

public:
	wchar_t _recvBuffer[1000];

private:
	SOCKET _clientSocket;
	SOCKADDR_IN _sockAddr;
	ConnectEvent* _connectEvent;
};