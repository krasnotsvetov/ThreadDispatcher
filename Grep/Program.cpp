#pragma once
#include <iostream>
#include "Grep.h"
using namespace::GrepTool;

int main(int argc, char* argv[])
{
	if (argc < 3) 
	{
		std::cout << "Usage <path> <word> <additional args>\n";
		std::cout << "Where <additional args> is \'-i\' for toggle off case sensitivity";
		return 0;
	}
	auto path = std::string(argv[1]);
	auto word = std::string(argv[2]);
	bool ignoreCaseSensitivity = false;
	
	if (argc == 4) 
	{
		ignoreCaseSensitivity = std::string(argv[3]).compare("-i") == 0;
	}
	auto result = Grep::Execute(path, word, ignoreCaseSensitivity, 1);
	auto t = result.get();
	for (auto& file : t)
	{
		std::cout << file << std::endl;
	}
	return 0;
}