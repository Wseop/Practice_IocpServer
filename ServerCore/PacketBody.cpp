#include "pch.h"
#include "PacketBody.h"
#include <iostream>

using namespace std;

Info::Info(int hp, int att, int def) : _hp(hp), _att(att), _def(def)
{
}

Info::~Info()
{
}

int Info::GetSize()
{
	return sizeof(Info);
}

void Info::ShowPacket()
{
	wcout << L"[Info] hp : " << _hp << ", att : " << _att << ", def : " << _def << endl;
}

Test::Test(int a, int b, int c, wstring msg) : _a(a), _b(b), _c(c)
{
	memcpy(_message, msg.c_str(), sizeof(WCHAR) * msg.size());
	_message[msg.size()] = 0;
}

Test::~Test()
{
}

int Test::GetSize()
{
	return sizeof(Test);
}

void Test::ShowPacket()
{
	wcout << L"[Test] a : " << _a << ", b : " << _b << ", c : " << _c << endl;
	wcout << L"[Test] msg : " << _message << endl;
}