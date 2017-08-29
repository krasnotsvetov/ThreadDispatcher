#ifndef H_GREP
#define H_GREP

#pragma once
#include <string>
#include <future>
#include <list>
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
		static std::list<std::future<void>> VisitDirectories(filesystem::path rootPath, std::string& word, bool ignoreCaseSensitivity, 
			ThreadDispatcher& dispatcher, std::vector<std::string>& result, std::shared_ptr<std::mutex> lockObj);
		static std::future<void> InspectFile(const std::experimental::filesystem::directory_entry& de, std::string& word, 
			bool ignoreCaseSensitivity, ThreadDispatcher& dispatcher, std::vector<std::string>& result, std::shared_ptr<std::mutex> lockObj);
	};
}
#endif