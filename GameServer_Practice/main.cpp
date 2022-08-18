#include "pch.h"
#include "ThreadManager.h"

using namespace std;

int main(void)
{
	for (int i = 0; i < 5; i++)
	{
		GThreadManager->CreateThread([=]() 
			{
				for (int j = 0; j < 10; j++)
				{
					wcout << L"[Thread #" << i + 1 << "] ThreadManager Test" << endl;

					this_thread::sleep_for(1s);
				}
			});
	}

	return 0;
}