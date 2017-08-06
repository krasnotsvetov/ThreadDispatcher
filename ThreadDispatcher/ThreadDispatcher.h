#pragma once
#ifndef H_THREADDISPATCHER
#define H_THREADDISPATCHER

#include <utility>
#include <vector>
#include <future>
#include <functional>
#include "DoNotExcept.h"


namespace Dispatcher 
{
	class ThreadDispatcher
	{
	public:
		ThreadDispatcher();
		
		template <typename T, typename... Args,
			typename retType = decltype(std::declval<T>()(std::declval<Args>()...))>
		std::future<retType> AddTask(T func, Args&&... args) {
			auto promise = std::make_shared<std::promise<retType>>();

			auto t = [func = std::move(func), args..., promise]() {
				
				DoNotExcept(
					[func = std::move(func), args..., promise]()
					{
						auto result = func(std::move(args)...);
						promise->set_value(result);
					},
					[promise](const std::exception_ptr& exception_ptr)
					{
						promise->set_exception(exception_ptr);
					}
				);

			};
			return promise->get_future();
		}

		~ThreadDispatcher();

	private:
		std::vector<std::function<void()>> tasks;
	};
}

#endif // !H_THREADDISPATCHER
