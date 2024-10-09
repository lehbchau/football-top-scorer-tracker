#include "thread_pool.h"

/// <summary>
/// Constructor to create a thread pool with specified number of threads
/// </summary>
/// <param name="numThreads">number of worker threads</param>
ThreadPool::ThreadPool(size_t numThreads) {
	stop = false;
	for (size_t i = 0; i < numThreads; ++i) {
		threads.emplace_back([this] { this->threadTask(); });
	}
}

/// <summary>
/// Function of worker thread that continuously picks up and executes tasks from task queue
/// </summary>
void ThreadPool::threadTask() {
	while (!stop) {
		std::packaged_task<League()> task;
		{
			std::unique_lock<std::mutex> lock(queueMtx);

			// Wait until stop or there's more task to do
			cv.wait(lock, [this] { return !tasks.empty() || stop; });

			if (stop && tasks.empty())
				return;

			// Get new task from queue
			task = std::move(tasks.front());
			tasks.pop();

		}
		task();
	}
}

/// <summary>
/// Enqueue new tasks into thread pool
/// </summary>
/// <param name="task">a callable object that represents the task to be executed by worker threads</param>
void ThreadPool::enqueue(std::packaged_task<League()> task) {
	// Lock the queue to add new tasks
	{
		std::unique_lock<std::mutex> lock(queueMtx);
		tasks.emplace(std::move(task));
	}

	// Notify one of the threads 
	cv.notify_one();
}

/// <summary>
/// Destructor to ensure all tasks are completed and threads are joined properly
/// </summary>
ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(queueMtx);
		stop = true;
	}

	cv.notify_all();
	for (auto& t : threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}