#pragma once
#include "ThreadDispatcher.h"
using namespace Dispatcher;

int main() {
	ThreadDispatcher dispatcher;
	dispatcher.AddTask([](int a) {return 0; }, 0);
}