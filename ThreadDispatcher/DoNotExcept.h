#pragma once
#ifndef DONOTEXCEPT
#define DONOTEXCEPT
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