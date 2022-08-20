#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "IocpCore.h"

ThreadManager* GThreadManager = nullptr;
IocpCore* GIocpCore = nullptr;

// Manager 클래스들의 생성, 소멸을 담당
class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GIocpCore = new IocpCore();
		SocketUtils::Init();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GIocpCore;
		SocketUtils::Clear();
	}
} GCoreGlobal;