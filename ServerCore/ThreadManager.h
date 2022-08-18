#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <functional>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void CreateThread(std::function<void(void)> callback);
	void Join();

private:
	std::vector<std::thread> _threads;
	std::mutex _mutex;
};

