#include "pch.h"
#include "ThreadManager.h"

using namespace std;

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::CreateThread(std::function<void(void)> callback)
{
	_threads.push_back(thread([=]()
		{
			callback();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}
