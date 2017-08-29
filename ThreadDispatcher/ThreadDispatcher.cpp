#include "ThreadDispatcher.h"
#include <thread>
#include <vector>

namespace Dispatcher {
	ThreadDispatcher::ThreadDispatcher(int threadCount) : isRunning(true)
	{
		for (int i = 0; i < threadCount; i++) {
			threads.push_back(std::thread(
				[this]() 
				{
					while (this->isRunning || tasks.size() > 0)
					{
						std::unique_lock<std::mutex> lock(this->mutex);
						this->taskAvailableNotifier.wait(lock,
							[this]()
							{
								return !(this->isRunning && this->tasks.empty());
							}
						);

						if (!this->isRunning) return;

						auto task = std::move(this->tasks.front());
						this->tasks.pop();

						task();
					}
				}
			));
		}
	} 

	ThreadDispatcher::~ThreadDispatcher()
	{
		isRunning = false;
		taskAvailableNotifier.notify_all();
		for (auto& t : threads) {
			t.join();
		}
	}
}