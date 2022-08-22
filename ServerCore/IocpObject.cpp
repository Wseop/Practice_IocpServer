﻿#include "pch.h"
#include "IocpObject.h"
#include "SocketUtils.h"
#include "IocpCore.h"
#include "IocpEvent.h"
#include "SendBuffer.h"

using namespace std;

IocpObject::IocpObject()
{
}

IocpObject::~IocpObject()
{
}

Listener::Listener()
{
	_listenSocket = SocketUtils::CreateSocket();
	assert(_listenSocket != INVALID_SOCKET);
	_serverAddr = SocketUtils::CreateSockAddr(L"127.0.0.1", 7777);
}

Listener::~Listener()
{
	SocketUtils::CloseSocket(_listenSocket);
	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		delete acceptEvent;
	}
}

void Listener::StartAccept()
{
	assert(SocketUtils::SetReuseAddress(_listenSocket, true));
	assert(SocketUtils::SetLinger(_listenSocket, 0, 0));
	assert(SocketUtils::Bind(_listenSocket, _serverAddr));
	assert(SocketUtils::Listen(_listenSocket));
	assert(GIocpCore->Register(_listenSocket));

	for (int i = 0; i < ACCEPT_COUNT; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->SetOwner(shared_from_this());
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = make_shared<Session>();
	acceptEvent->Init();
	acceptEvent->SetSession(session);
	assert(GIocpCore->Register(session->GetSocket()));

	DWORD bytesTransferred = 0;
	if (false == SocketUtils::AcceptEx(_listenSocket, session->GetSocket(), session->_recvBuffer, 
		0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytesTransferred, 
		static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			wcout << L"AcceptEx Error : " << errorCode << endl;
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = acceptEvent->GetSession();

	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _listenSocket))
	{
		wcout << L"SetUpdateAccpetSocket Error" << endl;
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);
	if (SOCKET_ERROR == getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&clientAddr), &addrLen))
	{
		wcout << L"getpeername Error" << endl;
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetAddr(clientAddr);
	wcout << L"Client Connected." << endl;

	// TODO. RegisterRecv, ProcessRecv
	RecvEvent* recvEvent = new RecvEvent();
	recvEvent->Init();
	recvEvent->SetOwner(session);

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(&session->_recvBuffer[0]);
	wsaBuf.len = 1000;
	DWORD dataTransferred = 0;
	DWORD flags = 0;
	WSARecv(session->GetSocket(), &wsaBuf, 1, &dataTransferred, &flags, recvEvent, nullptr);
	// 

	RegisterAccept(acceptEvent);
}

void Listener::Dispatch(IocpEvent* iocpEvent, DWORD bytesTransferred)
{
	assert(iocpEvent->GetType() == EventType::Accept);
	ProcessAccept(static_cast<AcceptEvent*>(iocpEvent));
}

Session::Session()
{
	_clientSocket = SocketUtils::CreateSocket();
	assert(_clientSocket != INVALID_SOCKET);
	_sockAddr = SocketUtils::CreateSockAddr(L"127.0.0.1", 7777);
}

Session::~Session()
{
	SocketUtils::CloseSocket(_clientSocket);
}

void Session::Dispatch(IocpEvent* iocpEvent, DWORD bytesTransferred)
{
	switch (iocpEvent->GetType())
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Disconnect:
		break;
	case EventType::Send:
		ProcessSend(static_cast<SendEvent*>(iocpEvent), bytesTransferred);
		break;
	case EventType::Recv:
		break;
	}
}

void Session::StartConnect()
{
	assert(SocketUtils::SetReuseAddress(_clientSocket, true));
	assert(SocketUtils::BindAnyAddress(_clientSocket));
	assert(GIocpCore->Register(_clientSocket));
	
	_connectEvent = new ConnectEvent();
	_connectEvent->Init();
	_connectEvent->SetOwner(shared_from_this());
	RegisterConnect();
}

void Session::RegisterConnect()
{
	DWORD bytesTransferred = 0;
	if (false == SocketUtils::ConnectEx(_clientSocket, reinterpret_cast<SOCKADDR*>(&_sockAddr), sizeof(_sockAddr),
		nullptr, 0, &bytesTransferred,
		static_cast<LPOVERLAPPED>(_connectEvent)))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			wcout << "ConnectEx Error : " << errorCode << endl;
			return;
		}
	}
}

void Session::ProcessConnect()
{
	_connectEvent->SetOwner(nullptr);
	delete _connectEvent;

	wcout << L"Connected to Server." << endl;
}

// SendEvent 생성 및 초기화
void Session::StartSend(SendBuffer* sendBuffer)
{
	SendEvent* sendEvent = new SendEvent();
	sendEvent->Init();
	sendEvent->SetOwner(shared_from_this());
	sendEvent->SetSendBuffer(sendBuffer);

	RegisterSend(sendEvent);
}

// WSASend 호출
void Session::RegisterSend(SendEvent* sendEvent)
{
	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(sendEvent->GetSendBuffer()->GetBuffer());
	wsaBuf.len = static_cast<ULONG>(sendEvent->GetSendBuffer()->WriteSize());

	DWORD bytesTransferred = 0;
	if (SOCKET_ERROR == WSASend(_clientSocket, &wsaBuf, 1, &bytesTransferred, 0, static_cast<LPOVERLAPPED>(sendEvent), nullptr))
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			wcout << L"WSASend Error : " << errorCode << endl;
			sendEvent->SetOwner(nullptr);
			delete sendEvent;
		}
	}
}

// SendEvent 후처리 (메모리 해제 등)
void Session::ProcessSend(SendEvent* sendEvent, DWORD bytesTransferred)
{
	if (bytesTransferred == 0)
	{
		wcout << L"Disconnected." << endl;
	}
	else
	{
		wcout << L"Data Send : " << bytesTransferred << L"bytes" << endl;
		wcout << L"SendBuffer Size : " << sendEvent->GetSendBuffer()->WriteSize() << endl;
	}

	sendEvent->SetOwner(nullptr);
	delete sendEvent;
}
