#include "Grep.h"
#include <iostream>
#include <string>
#include "PTestTool.h"

namespace filesystem = std::experimental::filesystem;

namespace GrepTool {

	std::future<std::vector<std::string>> Grep::Execute(std::string& path, std::string& word, bool ignoreCaseSensitivity, int threadCount = 8)
	{
		return std::async(
			[&]()
			{ 
				std::vector<std::string> result;
				std::mutex lockObj;
				{
					ThreadDispatcher dispatcher(threadCount);
					std::error_code errCode;
					if (filesystem::exists(path, errCode)) {
						
						VisitDirectories(path,word, ignoreCaseSensitivity, dispatcher, result, lockObj);
					}
				}
				return std::move(result);
			}
		);
	}

	///add task for folder visiting
	void Grep::VisitDirectories(filesystem::path rootPath, std::string& word, bool ignoreCaseSensitivity,  ThreadDispatcher& dispatcher, 
		std::vector<std::string>& result, std::mutex& lockObj) {
		for (auto& p : filesystem::directory_iterator(rootPath)) {
			auto status = p.status();
			if (filesystem::is_directory(status)) {
				VisitDirectories(p.path(), word, ignoreCaseSensitivity, dispatcher, result, lockObj);
			}
			else if (filesystem::is_regular_file(status) && !filesystem::is_symlink(status)) { 
				InspectFile(p, word, ignoreCaseSensitivity, dispatcher, result, lockObj); 
			}
		}
	}
		 
	///add task for file checking
	void Grep::InspectFile(const std::experimental::filesystem::directory_entry& de, std::string& word, bool ignoreCaseSensitivity, ThreadDispatcher& dispatcher, 
		std::vector<std::string>& result, std::mutex& lockObj)
	{  
  		dispatcher.AddTask(
			[&de, &word, ignoreCaseSensitivity, &result, &lockObj]()
			{
				int line = PTestTool::Test(de, word, ignoreCaseSensitivity);
				if (line > -1)
				{
					lockObj.lock();
					result.push_back(de.path().string());
					lockObj.unlock();
				}
			}
		);
	}
}