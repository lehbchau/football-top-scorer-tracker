#pragma once

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <functional>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <future>
#include "player_league_manager.h"

class ThreadPool {
public:
	ThreadPool(size_t numThreads);
	~ThreadPool();

	void enqueue(std::packaged_task<League()> task);

private:
	std::vector<std::thread> threads;
	std::queue < std::packaged_task<League()> > tasks;

	std::mutex queueMtx;
	std::condition_variable cv;
	std::atomic_bool stop;

	void threadTask();
};

#endif
