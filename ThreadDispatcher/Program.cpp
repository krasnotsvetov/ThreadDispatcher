#pragma once
#include "ThreadDispatcher.h"
using namespace Dispatcher;

int main() {
	ThreadDispatcher dispatcher;
	auto t = dispatcher.AddTask([](int a) {return ; }, 1);

}