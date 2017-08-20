#pragma once
#include "ThreadDispatcher.h"
using namespace Dispatcher;
#define TEST
#ifdef TEST
#include <functional>
#include <future>
#include "MoveableFunction.h"
#endif // TEMP



int main() {
	ThreadDispatcher dispatcher(5);
	auto t = dispatcher.AddTask([](int a) {return  ++a; }, 1);
	auto result = t.get();

	int x = 5;
	std::promise<void> promise;
	//don't work
	//std::function<void()> testVoid([p = std::move(promise)]() {});
	//work
	Dispatcher::Utils::MoveableFunction<void()> zzz([p = std::move(promise)]() {});
}