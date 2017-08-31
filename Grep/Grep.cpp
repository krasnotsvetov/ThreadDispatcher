
#pragma once
#include "Grep.h"


namespace Grep {
	namespace {
		///add task for file checking
		auto InspectFile(const std::experimental::filesystem::directory_entry& de,const std::string& word,const bool ignoreCaseSensitivity, ThreadDispatcher& dispatcher)
		{  
			return dispatcher.AddTask([&, de]()
				{
					int line = PTestTool::Test(de, word, ignoreCaseSensitivity);
					if (line > -1)
					{
						return de.path().string();
					}
					return std::string("");
				}
			);
		}

		///add task for folder visiting
		std::list<std::future<std::string>> VisitDirectories(const filesystem::path rootPath,const std::string& word,const bool ignoreCaseSensitivity,  ThreadDispatcher& dispatcher)
		{
			std::list<std::future<std::string>> futures;
			for (auto& p : filesystem::directory_iterator(rootPath)) 
			{
				auto status = p.status();
				if (filesystem::is_directory(status)) 
				{
					futures.splice(futures.end(), VisitDirectories(p.path(), word, ignoreCaseSensitivity, dispatcher));
				}
				else if (filesystem::is_regular_file(status) && !filesystem::is_symlink(status)) 
				{ 
					futures.push_back(InspectFile(p, word, ignoreCaseSensitivity, dispatcher)); 
				}
			}
			return futures;
		}
	}

	std::future<std::vector<std::string>> Execute(const std::string& path,const std::string& word,const bool ignoreCaseSensitivity,const int threadCount = 8)
	{
		return std::async(
			[=]()
			{ 
				std::vector<std::string> result;
				{
					ThreadDispatcher dispatcher(threadCount);
					std::error_code errCode;
					if (filesystem::exists(path, errCode)) 
					{
						auto futures = VisitDirectories(path,word, ignoreCaseSensitivity, dispatcher);
						for (auto& future : futures) 
						{
							auto res = future.get();
							if (res != "") result.push_back(res);
						}
					}
				}
				return result;
			}
		);
	}
}