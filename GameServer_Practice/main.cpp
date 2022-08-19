#include "pch.h"
#include "ThreadManager.h"
#include "SocketUtils.h"

using namespace std;

int main(void)
{
	GThreadManager->CreateThread([=]()
		{
			// SocketUtils Test

			SOCKET listenSocket = SocketUtils::CreateSocket();
			assert(listenSocket != INVALID_SOCKET);
			
			SOCKADDR_IN serverAddr = SocketUtils::CreateSockAddr(L"127.0.0.1", 7777);
			assert(SocketUtils::Bind(listenSocket, serverAddr));
			assert(SocketUtils::Listen(listenSocket));

			HANDLE iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			assert(iocpHandle != NULL);
			assert(CreateIoCompletionPort(reinterpret_cast<HANDLE>(listenSocket), iocpHandle, 0, 0));

			SOCKET clientSocket = SocketUtils::CreateSocket();
			assert(clientSocket != INVALID_SOCKET);

			char lpOutputBuf[1024];
			int outBufLen = 1024;
			DWORD dwBytes;
			WSAOVERLAPPED overlapped;
			memset(&overlapped, 0, sizeof(overlapped));

			assert(CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), iocpHandle, 0, 0));

			if (false == SocketUtils::AcceptEx(listenSocket, clientSocket, lpOutputBuf, outBufLen - ((sizeof(SOCKADDR_IN) + 16) * 2), sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, &overlapped))
			{
				int errorCode = WSAGetLastError();
				if (errorCode != WSA_IO_PENDING)
				{
					cout << "AcceptEx Error : " << errorCode << endl;
					return 0;
				}
			}

			ULONG_PTR key = 0;
			if (GetQueuedCompletionStatus(iocpHandle, &dwBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&overlapped), INFINITE))
			{
				assert(SocketUtils::SetUpdateAcceptSocket(clientSocket, listenSocket));
				cout << "Client Connected. Test End" << endl;

				this_thread::sleep_for(5s);
			}
			else
			{
				cout << "Dispatch Error" << endl;
				return 0;
			}
		});

	return 0;
}