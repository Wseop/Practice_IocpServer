#include "pch.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "IocpCore.h"
#include "IocpObject.h"
#include "SendBuffer.h"
#include "TestPacket.h"
#include <vector>

using namespace std;

// Send Test w/ SendBuffer
int main(void)
{
	vector<shared_ptr<Session>> sessions;

	// 클라이언트 5개 연결 (session 5개)
	for (int i = 0; i < 5; i++)
	{
		shared_ptr<Session> session = make_shared<Session>();
		sessions.push_back(session);
		session->StartConnect();
	}

	for (int i = 0; i < 3; i++)
	{
		GThreadManager->CreateThread([=]()
			{
				while (true)
				{
					GIocpCore->Dispatch();
				}
			});
	
	}

	static int a = 1;
	GThreadManager->CreateThread([=]()
		{
			while (true)
			{
				TestPacket packet(a, a + 1, a + 2);
				a++;

				for (shared_ptr<Session> session : sessions)
				{
					SendBuffer* sendBuffer = new SendBuffer(reinterpret_cast<BYTE*>(&packet), sizeof(TestPacket));
					session->StartSend(sendBuffer);
				}

				this_thread::sleep_for(3s);
			}
		});


	return 0;
}