#pragma once

#include "PacketBody.h"

class PacketHeader;

class PacketManager
{
public:
	static BYTE* CreatePacket(PacketType type, BYTE* data, int dataSize);
	static PacketHeader* DetachHeader(BYTE** data, DWORD bytesTransferred);

public:
	static uint64_t id;
};

