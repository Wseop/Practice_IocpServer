#include "pch.h"
#include "SocketUtils.h"

LPFN_CONNECTEX SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX SocketUtils::AcceptEx = nullptr;

// WSAStartup 및 함수 바인딩
void SocketUtils::Init()
{
	WSAData wsaData;
	assert(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);

	SOCKET dummySocket = CreateSocket();
	assert(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	assert(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	assert(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	CloseSocket(dummySocket);
}

void SocketUtils::Clear()
{
	WSACleanup();
}

bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), &bytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

void SocketUtils::CloseSocket(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		closesocket(socket);
	socket = INVALID_SOCKET;
}

SOCKADDR_IN SocketUtils::CreateSockAddr(std::wstring ip, u_short port)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	InetPtonW(AF_INET, ip.c_str(), &sockAddr.sin_addr);
	sockAddr.sin_port = htons(port);

	return sockAddr;
}

bool SocketUtils::SetLinger(SOCKET socket, u_short onoff, u_short time)
{
	LINGER linger;
	linger.l_onoff = onoff;
	linger.l_linger = time;
	return SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&linger), sizeof(linger));
}

bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{
	return SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&flag), sizeof(flag));
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int size)
{
	return SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&size), sizeof(size));
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int size)
{
	return SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&size), sizeof(size));
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, TCP_NODELAY, reinterpret_cast<const char*>(&flag), sizeof(flag));
}

// ListenSocket의 특성을 연결된 ClientSocket에 적용
bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SOCKET_ERROR != setsockopt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<const char*>(&listenSocket), sizeof(listenSocket));
}

bool SocketUtils::Bind(SOCKET socket, SOCKADDR_IN sockAddr)
{
	return SOCKET_ERROR != bind(socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(SOCKADDR_IN));
}

bool SocketUtils::BindAnyAddress(SOCKET socket)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons(0);
	return SOCKET_ERROR != bind(socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr));
}

bool SocketUtils::Listen(SOCKET socket, int backlog)
{
	return SOCKET_ERROR != listen(socket, backlog);
}

