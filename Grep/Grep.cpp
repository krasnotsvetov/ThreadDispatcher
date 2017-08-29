#include "Grep.h"
#include "PTestTool.h"

namespace filesystem = std::experimental::filesystem;

namespace GrepTool {

	std::future<std::vector<std::string>> Grep::Execute(std::string& path, std::string& word, bool ignoreCaseSensitivity, int threadCount = 8)
	{
		return std::async(
			[=]() mutable
			{ 
				std::vector<std::string> result;
				std::mutex lockObj;
				{
					ThreadDispatcher dispatcher(threadCount);
					std::error_code errCode;
					if (filesystem::exists(path, errCode)) 
					{
						auto futures = VisitDirectories(path,word, ignoreCaseSensitivity, dispatcher, result, std::make_shared<std::mutex>());
						for (auto& future : futures) 
						{
							future.get();
						}
					}
				}
				return std::move(result);
			}
		);
	}

	///add task for folder visiting
	std::list<std::future<void>> Grep::VisitDirectories(filesystem::path rootPath, std::string& word, bool ignoreCaseSensitivity,  ThreadDispatcher& dispatcher, 
		std::vector<std::string>& result, std::shared_ptr<std::mutex> lockObj)
	{
		std::list<std::future<void>> futures;
		for (auto& p : filesystem::directory_iterator(rootPath)) 
		{
			auto status = p.status();
			if (filesystem::is_directory(status)) 
			{
				futures.splice(futures.end(), VisitDirectories(p.path(), word, ignoreCaseSensitivity, dispatcher, result, lockObj));
			}
			else if (filesystem::is_regular_file(status) && !filesystem::is_symlink(status)) 
			{ 
				futures.push_back(InspectFile(p, word, ignoreCaseSensitivity, dispatcher, result, lockObj)); 
			}
		}
		return futures;
	}
		 
	///add task for file checking
	std::future<void> Grep::InspectFile(const std::experimental::filesystem::directory_entry& de, std::string& word, bool ignoreCaseSensitivity, ThreadDispatcher& dispatcher, 
		std::vector<std::string>& result, std::shared_ptr<std::mutex> lockObj)
	{  
		return dispatcher.AddTask([&, de, lockObj]() mutable
			{
				int line = PTestTool::Test(de, word, ignoreCaseSensitivity);
				if (line > -1)
				{
					std::unique_lock<std::mutex> lock(*lockObj);
					result.push_back(de.path().string());
				}
			}
		);
	}
}