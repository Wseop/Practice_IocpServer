#include "pch.h"
#include "ThreadManager.h"
#include "SocketUtils.h"

using namespace std;

int main(void)
{
	GThreadManager->CreateThread([=]()
		{
			// SocketUtils Test

			SOCKET socket = SocketUtils::CreateSocket();
			assert(socket != INVALID_SOCKET);

			SOCKADDR_IN serverAddr = SocketUtils::CreateSockAddr(L"127.0.0.1", 7777);

			HANDLE iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			assert(iocpHandle != NULL);

			if (SocketUtils::SetReuseAddress(socket, true) == false)
			{
				cout << "SetReuseAddress Error : " << WSAGetLastError() << endl;
				return 0;
			}

			if (SocketUtils::BindAnyAddress(socket) == false)
			{
				cout << "Bind Error : " << WSAGetLastError() << endl;
				return 0;
			}

			WSAOVERLAPPED overlapped;
			memset(&overlapped, 0, sizeof(overlapped));

			assert(CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), iocpHandle, 0, 0));

			DWORD numOfBytes = 0;
			if (false == SocketUtils::ConnectEx(socket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr), nullptr, 0, &numOfBytes, &overlapped))
			{
				int errorCode = WSAGetLastError();
				if (errorCode != WSA_IO_PENDING)
				{
					cout << "ConnectEx Error : " << errorCode << endl;
					return 0;
				}
			}

			ULONG_PTR key = 0;
			GetQueuedCompletionStatus(iocpHandle, &numOfBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&overlapped), INFINITE);
		});

	return 0;
}