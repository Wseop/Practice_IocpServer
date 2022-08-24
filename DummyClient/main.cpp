#include "pch.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "IocpCore.h"
#include "IocpObject.h"
#include "SendBuffer.h"
#include "PacketManager.h"
#include "PacketHeader.h"
#include "PacketBody.h"
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
				if (a & 1)
				{
					// Info Packet
					Info packet(a, a + 1, a + 2);

					BYTE* packetPtr = PacketManager::CreatePacket(PacketType::Info, reinterpret_cast<BYTE*>(&packet), packet.GetSize());

					for (shared_ptr<Session> session : sessions)
					{
						SendBuffer* sendBuffer = new SendBuffer(packetPtr, sizeof(PacketHeader) + packet.GetSize());
						session->StartSend(sendBuffer);
					}
					delete packetPtr;
				}
				else
				{
					// Test Packet
					Test packet(a, a + 1, a + 2, L"This is test packet");

					BYTE* packetPtr = PacketManager::CreatePacket(PacketType::Test, reinterpret_cast<BYTE*>(&packet), packet.GetSize());

					for (shared_ptr<Session> session : sessions)
					{
						SendBuffer* sendBuffer = new SendBuffer(packetPtr, sizeof(PacketHeader) + packet.GetSize());
						session->StartSend(sendBuffer);
					}
					delete packetPtr;
				}

				a++;
				this_thread::sleep_for(10ms);
			}
		});


	return 0;
}
