#pragma once
#ifndef H_THREADDISPATCHER
#define H_THREADDISPATCHER

#include <utility>
#include <vector>
#include <future>
#include <functional>
#include <type_traits>

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

			tasks.emplace_back([func = std::move(func), args..., promise]() {
				
				DoNotExcept(
					[func = std::move(func), args..., promise]()
					{
						PutToPromiseOrIgnore(std::move(func), promise, std::move(args)...);
					},
					[promise](const std::exception_ptr& exception_ptr)
					{
						promise->set_exception(exception_ptr);
					}
				);

			});
			return promise->get_future();
		}

		~ThreadDispatcher();

	private:
		
		template<typename T, typename... Args,
			typename retType = decltype(std::declval<T>()(std::declval<Args>()...))>
		static void PutToPromiseOrIgnore(T func, std::shared_ptr<std::promise<retType>> promise, Args&&... args) {
			
			promise->set_value(func(std::move(args)...));
		}

		template<typename T, typename... Args>
		static void PutToPromiseOrIgnore(T func, std::shared_ptr<std::promise<void>> promise, Args&&... args) {

			func(std::move(args)...);
			promise->set_value();
		}

		std::vector<std::function<void()>> tasks;
	};
}

#endif // !H_THREADDISPATCHER
