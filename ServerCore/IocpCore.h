#pragma once

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }
	bool Register(SOCKET socket);
	void Dispatch(DWORD timeoutMs = INFINITE);

private:
	HANDLE _iocpHandle;
};

