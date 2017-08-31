#pragma once
#ifndef GREP
#define GREP

#include <string>
#include <future>
#include <list>
#include <experimental/filesystem>
#include "../ThreadDispatcher/ThreadDispatcher.h"
#include "PTestTool.h"

using namespace Dispatcher;
namespace filesystem = std::experimental::filesystem;

namespace Grep {
	std::future<std::vector<std::string>> Execute(const std::string& path,const std::string& word,const bool ignoreCaseSensitivity,const int threadCount);
}

#endif