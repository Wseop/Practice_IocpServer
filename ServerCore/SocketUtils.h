#pragma once

class SocketUtils
{
public:
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_DISCONNECTEX DisconnectEx;
	static LPFN_ACCEPTEX AcceptEx;

public:
	static void Init();
	static void Clear();
	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);

	static SOCKET CreateSocket();
	static void CloseSocket(SOCKET& socket);
	static SOCKADDR_IN CreateSockAddr(std::wstring ip, u_short port);

	static bool SetLinger(SOCKET socket, u_short onoff, u_short linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetRecvBufferSize(SOCKET socket, int size);
	static bool SetSendBufferSize(SOCKET socket, int size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	static bool Bind(SOCKET socket, SOCKADDR_IN sockAddr);
	static bool BindAnyAddress(SOCKET socket);

	static bool Listen(SOCKET socket, int backlog = SOMAXCONN);
};

