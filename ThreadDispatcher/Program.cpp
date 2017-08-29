#pragma once
#include "ThreadDispatcher.h"
#include <iostream>
using namespace Dispatcher;
#define TEST
#ifdef TEST
#include <functional>
#include <future>
#include "MoveableFunction.h"
#endif // TEMP


void print_int (std::future<int>& fut) {
  int x = fut.get();
  std::cout << "value: " << x << '\n';
}
int main() {
	/*ThreadDispatcher dispatcher(5);
	auto t = dispatcher.AddTask([](int a) {return  ++a; }, 1);

	auto result = t.get();

	int x = 5;
	std::promise<void> promise;
	//don't work
	//std::function<void()> testVoid([p = std::move(promise)]() {});
	//work
	Dispatcher::Utils::MoveableFunction<int(int)> zzz([p = std::move(promise)](int x) {return ++x;});
	std::vector<Dispatcher::Utils::MoveableFunction<int(int)>> vec;
	vec.push_back([p = std::move(promise)](int x) {return ++x;});
	int t2 = vec[0](x);*/
}