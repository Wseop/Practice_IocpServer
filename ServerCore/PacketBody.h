#pragma once

enum class PacketType
{
	Info = 0,
	Test = 999,
};

class Info
{
public:
	Info(int hp, int att, int def);
	~Info();

	int GetSize();
	void ShowPacket();

private:
	int _hp;
	int _att;
	int _def;
};

class Test
{
public:
	Test(int a, int b, int c, std::wstring msg);
	~Test();

	int GetSize();
	void ShowPacket();

private:
	int _a, _b, _c;
	WCHAR _message[100];
};