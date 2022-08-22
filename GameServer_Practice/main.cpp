#include "pch.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "IocpCore.h"
#include "IocpObject.h"

using namespace std;

int main(void)
{
	shared_ptr<Listener> listener = make_shared<Listener>();
	listener->StartAccept();

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