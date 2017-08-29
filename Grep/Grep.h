#ifndef H_GREP
#define H_GREP

#pragma once
#include <string>
#include <future>
#include <experimental/filesystem>
#include "../ThreadDispatcher/ThreadDispatcher.h"

using namespace Dispatcher;
namespace filesystem = std::experimental::filesystem;

namespace GrepTool {
	class Grep {
	
	public:
		static std::future<std::vector<std::string>> Execute(std::string& path, std::string& word, 
			bool ignoreCaseSensitivity, int threadCount);
	
	private:
		static void VisitDirectories(filesystem::path rootPath, std::string& word, bool ignoreCaseSensitivity, 
			ThreadDispatcher& dispatcher, std::vector<std::string>& result, std::mutex& lockObj);
		static void InspectFile(const std::experimental::filesystem::directory_entry& de, std::string& word, 
			bool ignoreCaseSensitivity, ThreadDispatcher& dispatcher, std::vector<std::string>& result, std::mutex& lockObj);
	};
}
#endif