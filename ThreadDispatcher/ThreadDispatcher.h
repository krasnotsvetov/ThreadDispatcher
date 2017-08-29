#pragma once
#ifndef H_THREADDISPATCHER
#define H_THREADDISPATCHER

#include <utility>
#include <vector>
#include <future>
#include <functional>
#include <type_traits>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <queue>

#include "DoNotExcept.h"
#include "MoveableFunction.h"

namespace Dispatcher 
{
	class ThreadDispatcher
	{
	public:
		ThreadDispatcher(int threadCount);
		
		template <typename T, typename... Args,
			typename retType = decltype(std::declval<T>()(std::declval<Args>()...))>
		std::future<retType> AddTask(T func, Args&&... args)
		{
			std::unique_lock<std::mutex> lock(this->mutex);
			if (!isRunning) return std::future<void>();

			std::promise<retType> promise;
			auto future = promise.get_future();
			tasks.emplace([func = std::move(func), args..., promise = std::move(promise)]() mutable {
				
				DoNotExcept(
					[&]()
				{
					SetPromiseValue(func, promise, std::move(args)...);
				},
					[&promise](const std::exception_ptr& exception_ptr)
				{
					promise.set_exception(exception_ptr);
				}
				);
			});

			taskAvailableNotifier.notify_one();
			return future;
		}

		~ThreadDispatcher();

	private:
		template<typename T, typename... Args,
			typename retType = decltype(std::declval<T>()(std::declval<Args>()...))>
		static void SetPromiseValue(T& func, std::promise<retType>& promise, Args&&... args) {
			
			promise.set_value(func(std::move(args)...));
		}

		template<typename T, typename... Args>
		static void SetPromiseValue(T& func, std::promise<void>& promise, Args&&... args) {

			func(std::move(args)...);
			promise.set_value();
		}

		std::queue<Utils::MoveableFunction<void()>> tasks;
		std::vector<std::thread> threads;
		std::atomic<bool> isRunning;
		std::condition_variable taskAvailableNotifier;
		std::mutex mutex;
		std::mutex taskAddMutex;
	};
}

#endif // !H_THREADDISPATCHER
