#include "pch.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "IocpCore.h"
#include "IocpObject.h"
#include "SendBuffer.h"
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

	// 각 session마다 4바이트씩 크기가 증가하는 vector를 서버로 전달
	GThreadManager->CreateThread([=]()
		{
			vector<int> data;

			while (true)
			{
				data.push_back(1);

				for (shared_ptr<Session> session : sessions)
				{
					SendBuffer* sendBuffer = new SendBuffer(reinterpret_cast<BYTE*>(data.data()), data.size() * sizeof(int));
					session->StartSend(sendBuffer);
				}

				this_thread::sleep_for(3s);
			}
		});


	return 0;
}