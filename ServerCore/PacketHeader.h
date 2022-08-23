#pragma once

#include "PacketBody.h"

class PacketHeader
{
public:
	PacketHeader(uint64_t id, PacketType type, int size);
	~PacketHeader();

	uint64_t GetId() { return _id; }
	PacketType GetType() { return _type; }
	int GetSize() { return _size; }

private:
	uint64_t _id;
	PacketType _type;
	int _size;
};

