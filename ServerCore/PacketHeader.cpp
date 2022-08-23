#include "pch.h"
#include "PacketHeader.h"

PacketHeader::PacketHeader(uint64_t id, PacketType type, int size) : _id(id), _type(type), _size(sizeof(PacketHeader) + size)
{
}

PacketHeader::~PacketHeader()
{
}
