#pragma once
#include "ThreadDispatcher.h"
using namespace Dispatcher;

int main() {
	ThreadDispatcher dispatcher(5);
	auto t = dispatcher.AddTask([](int a) {return  ++a; }, 1);
	auto result = t.get();
}