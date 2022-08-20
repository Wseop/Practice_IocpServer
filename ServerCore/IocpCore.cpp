#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"
#include "IocpObject.h"

using namespace std;

IocpCore::IocpCore()
{
	_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	assert(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	CloseHandle(_iocpHandle);
}

bool IocpCore::Register(SOCKET socket)
{
	return CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), _iocpHandle, 0, 0);
}

void IocpCore::Dispatch(DWORD timeoutMs)
{
	DWORD bytesTransferred = 0;
	ULONG_PTR dummyKey = 0;
	IocpEvent* iocpEvent = nullptr;

	if (GetQueuedCompletionStatus(_iocpHandle, &bytesTransferred, &dummyKey, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		shared_ptr<IocpObject> iocpObject = iocpEvent->GetOwner();
		iocpObject->Dispatch(iocpEvent, bytesTransferred);
	}
	else
	{
		int errorCode = WSAGetLastError();
		wcout << "Dispatch Error : " << errorCode << endl;
	}
}
