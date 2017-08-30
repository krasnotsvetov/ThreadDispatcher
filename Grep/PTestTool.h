#pragma once
#ifndef PTEST_TOOL
#define PTEST_TOOL

#include <experimental/filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

namespace GrepTool {
	class PTestTool {
	public:

		static int Test(const std::experimental::filesystem::directory_entry& entry, std::string& word, bool ignoreCaseSensitivity) ;
	private:
		static bool pFunctionTest(std::string & str, std::string & word, bool const caseSensitivity);
		static char transformChar(char const c, bool const caseSensitivity);
	};
}
#endif