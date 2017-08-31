#pragma once
#ifndef PTEST_TOOL
#define PTEST_TOOL

#include <experimental/filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

namespace Grep {
	namespace PTestTool {
		int Test(const std::experimental::filesystem::directory_entry&  entry,const std::string & word,const bool ignoreCaseSensitivity) noexcept;
	}
}
#endif