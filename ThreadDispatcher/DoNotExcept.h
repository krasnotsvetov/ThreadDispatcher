#pragma once
#ifndef H_DONOTEXCEPT
#define H_DONOTEXCEPT
#include <functional>
#include <exception>

static void DoNotExcept(std::function<void()> action, 
	std::function<void(std::exception_ptr exception)> exceptionHandler) {
	try {
		action();
	}
	catch (...) {
		exceptionHandler(std::current_exception());
	}
}
#endif