#include "pch.h"
#include "PacketManager.h"
#include "PacketHeader.h"
#include <iostream>

using namespace std;

uint64_t PacketManager::id = 1;

// PacketHeader를 생성 후 PacketBody와 연결
BYTE* PacketManager::CreatePacket(PacketType type, BYTE* data, int dataSize)
{
	PacketHeader header(id++, type, dataSize);

	BYTE* newPacket = new BYTE[sizeof(PacketHeader) + dataSize];
	memcpy(newPacket, &header, sizeof(PacketHeader));
	memcpy(newPacket + sizeof(PacketHeader), data, dataSize);
	return newPacket;
}

PacketHeader* PacketManager::DetachHeader(BYTE** data, DWORD bytesTransferred)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(*data);

	if (header->GetSize() != bytesTransferred)
	{
		wcout << L"Packet Size Error" << endl;
		return nullptr;
	}
	(*data) += sizeof(PacketHeader);

	return header;
}
