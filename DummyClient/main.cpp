#include "pch.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "IocpCore.h"
#include "IocpObject.h"
#include <vector>

using namespace std;

int main(void)
{
	// IocpCore, IocpObject, IocpEvent - Connect Test
	vector<shared_ptr<Session>> sessions;

	for (int i = 0; i < 5; i++)
	{
		shared_ptr<Session> session = make_shared<Session>();
		sessions.push_back(session);
		session->StartConnect();
	}

	for (int i = 0; i < 10; i++)
	{
		GThreadManager->CreateThread([=]() 
			{
				while (true)
				{
					GIocpCore->Dispatch();
				}
			});
	}

	return 0;
}